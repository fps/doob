from PyQt4.QtGui import *
from PyQt4.QtCore import *

import json
import tempfile
import os
import dbus
import uuid
import math

class node(QGraphicsRectItem):
	DEAD = 1
	ALIVE = 2
	ACTIVATED = 3
	
	IS_PORT = 1
	IS_PORT_CONNECTOR = 2
	IS_LADSPA = 3
	IS_MIDI_NOTE = 4
	
	NODE_WIDTH = 120
	NODE_HEIGHT = 120
	
	CONNECTOR_SIZE = 3
	
	def __init__(self, the_uuid):
		QGraphicsRectItem.__init__(self)
		
		if None == the_uuid:
			self.uuid = uuid.uuid4().hex
		else:
			self.uuid = the_uuid
			
		self.monospace_font = QFont("monospace")
		self.monospace_font_metric = QFontMetrics(self.monospace_font)
		
		# print "node"
		
		self.state = node.DEAD
		self.base_brush_color = [48*2, 85*2, 67*2]
		self.selected_color = [200, 179, 110]
		self.current_color = self.base_brush_color
		self.highlighed_brush_colors = { node.DEAD: [128, 0, 0], node.ALIVE: [0, 60, 0], node.ACTIVATED: [30, 30, 30] }
		self.highlight_timer_count = 0
		
		self.setRect(0, 0, node.NODE_WIDTH, node.NODE_HEIGHT)
		self.name = "node"

		self.setBrush(QBrush(QColor(48*2, 85*2, 67*2)))
		pen = QPen(QColor(48*3, 85*3, 67*3))
		pen.setWidth(4)
		self.setPen(pen)
		
		self.setFlags(QGraphicsItem.ItemIsMovable | QGraphicsItem.ItemSendsGeometryChanges |QGraphicsItem.ItemIsSelectable)
		
		self.glow_timer = QTimer()
		self.glow_timer.setSingleShot(False)
		self.glow_timer.timeout.connect(self.glow_timer_timedout)
		self.glow_timer.setInterval(250)
		#self.glow_timer.start()
	
	def color(self, array):
		capped = map(lambda x: min(255,x), array)
		return QColor(capped[0], capped[1], capped[2])
		
	def glow_timer_timedout(self):
		# print self.highlight_timer_count
		self.highlight_timer_count += 1
		self.highlight_timer_count %= 10

		if self.highlight_timer_count % 10 == 0:
			self.setBrush(QBrush(self.color(map(sum, zip(self.highlighed_brush_colors[self.state], self.current_color)))))
		if not self.highlight_timer_count % 10 == 0:
			self.setBrush(QBrush(self.color(self.current_color)))
	
	def itemChange(self, change, value):
		# print "change"
		if change == QGraphicsItem.ItemPositionChange and not self.scene() == None:
			new_pos = value.toPointF()
			new_pos.setX(math.floor(new_pos.x() / self.monospace_font_metric.lineSpacing()) * (self.monospace_font_metric.lineSpacing()))
			new_pos.setY(math.floor(new_pos.y() / self.monospace_font_metric.lineSpacing()) * (self.monospace_font_metric.lineSpacing()))
			# print "pos"
			return new_pos
		
		if change == QGraphicsItem.ItemSelectedChange:
			if True == value.toBool():
				self.current_color = self.selected_color
				self.setBrush(QBrush(self.color(self.current_color)))
			else:
				self.current_color = self.base_brush_color
				self.setBrush(QBrush(self.color(self.current_color)))

		return QGraphicsItem.itemChange(self, change, value)

class process_node(node):
	def __init__(self, the_uuid):
		node.__init__(self, the_uuid)

		self.process = QProcess()
		self.process.readyReadStandardOutput.connect(self.process_ready_read_stdout)
		self.process.readyReadStandardError.connect(self.process_ready_read_stderr)
		self.process.error.connect(self.process_error)
		self.process.finished.connect(self.process_finished)

	def make_service_name(self, service_name):
		return service_name + self.uuid
		
	def start(self):
		pass
	
	def process_error(self):
		self.state = node.DEAD
		
	def process_finished(self):
		self.state = node.DEAD
	
	def process_ready_read_stdout(self):
		print ("stdout", self.process.readAllStandardOutput())

	def process_ready_read_stderr(self):
		print ("stdout", self.process.readAllStandardError())

	def stop(self):
		print ("Stopping process:", self.library, self.label)

		if self.process.state() == QProcess.Running:
			print "kill process"
			os.kill(self.process.pid(), 2)
			
			print "wait until finished"
			self.process.waitForFinished()

			print("stdout", self.process.readAllStandardOutput())
			print("stderr", self.process.readAllStandardError())


class jack_client_node(process_node):
	def __init__(self, the_uuid):
		process_node.__init__(self, the_uuid)
		self.jack_client_ports = []

class connection(QGraphicsLineItem):
	def __init__(self, source, source_port_index, sink, sink_port_index):
		QGraphicsLineItem.__init__(self)
		

class ladspa_node(jack_client_node):
	def to_json(self):
		return {'__class__': 'ladspa_node', '__value__': { 'library': str(self.library), 'label': str(self.label), 'x': self.x(), 'y': self.y(), 'uuid': self.uuid } }

	def __init__(self, library, label, the_uuid = None):
		jack_client_node.__init__(self, the_uuid)

		# print type(self)
		self.library = library
		self.label = label
		
		self.jack_name = QGraphicsTextItem()
		self.jack_name.setParentItem(self)
		
		self.plugin_name = QGraphicsTextItem()
		self.plugin_name.setParentItem(self)
		self.plugin_name.setPlainText(self.library + "\n" + self.label)
		self.plugin_name.setTextWidth(node.NODE_WIDTH)
		
		self.ports = []
		self.sink_ports = []
		self.source_ports = []
		self.port_infos = []

		self.adjust_children()

		self.service_name = self.make_service_name("io.fps.doob.Ladspa")

		self.bus = dbus.SessionBus()
		self.bus.add_signal_receiver(handler_function=self.process_port_changed, signal_name="PortsChanged", bus_name=self.service_name, dbus_interface="io.fps.doob.ladspa")
		self.bus.add_signal_receiver(handler_function=self.process_jack_client_name_changed, signal_name="JackNameChanged", bus_name=self.service_name, dbus_interface="io.fps.doob.jack_client")
		self.bus.add_signal_receiver(handler_function=self.process_plugin_name_changed, signal_name="PluginNameChanged", bus_name=self.service_name, dbus_interface="io.fps.doob.ladspa")
			
		self.start()

	def is_port_control(self, arg):
		return arg & 0x8
	
	def is_port_input(self, arg):
		return arg & 0x1
	
	def process_jack_client_name_changed(self, arg):
		print ("jack_client name: ", arg)
		#self.jack_name.setPlainText(str(arg))
		self.adjust_children()
		pass

	def process_plugin_name_changed(self, arg):
		print ("plugin name: ", arg)
		self.plugin_name.setPlainText(str(arg))
		self.adjust_children()
		pass

	def adjust_children(self):
		# Remove all ports if they exist previously
		for port in self.ports:
			port.scene().removeItem(port)
		
		self.ports = []
		self.source_ports = []
		self.sink_ports = []

		#self.jack_name.adjustSize()
		#self.plugin_name.adjustSize()
		self.plugin_name.setY(self.jack_name.boundingRect().height())
		plugin_name_rect = self.plugin_name.boundingRect()
		y_offset = plugin_name_rect.y() + plugin_name_rect.height()
		
		for port in self.port_infos:
			portname = QGraphicsTextItem()
			portname.setData(node.IS_PORT, QVariant(True))
			portname.setParentItem(self)
			portname.setPlainText(port[0])
			portname.setTextWidth(node.NODE_WIDTH - 20)
			
			port_connector = QGraphicsRectItem()
			port_connector.setData(node.IS_PORT_CONNECTOR, QVariant(True))
			port_connector.setRect(-node.CONNECTOR_SIZE, -node.CONNECTOR_SIZE, 2*node.CONNECTOR_SIZE, 2*node.CONNECTOR_SIZE)
			port_connector.setParentItem(portname)
			port_connector.setBrush(QBrush(QColor(255, 255, 255, 128)))
			port_connector.setPen(QPen(QColor(0, 0, 0, 0)))
			port_connector.setY(portname.boundingRect().height() / 2)

			name_background = QGraphicsRectItem()
			name_background.setParentItem(portname)
			name_background.setRect(portname.boundingRect())
			name_background.setPen(QPen(QColor(0,0,0,0)))
			if self.is_port_control(port[1]):
				name_background.setBrush(QBrush(QColor(0,0,255,20)))
			else:
				name_background.setBrush(QBrush(QColor(255,0,0,20)))

			self.ports.append(portname)
			if self.is_port_input(port[1]):
				port_connector.setX(0)
				self.sink_ports.append(portname)
			else:
				portname.setX(node.NODE_WIDTH - portname.boundingRect().width())
				port_connector.setX(portname.boundingRect().width())
				self.source_ports.append(portname)

			portname.setY(y_offset)
			
			y_offset += portname.boundingRect().height() + 4

		the_bounding_rect = self.childrenBoundingRect()
		self.setRect(0, 0, node.NODE_WIDTH, the_bounding_rect.height())


	def process_port_changed(self, arg):
		print ("######################### process ports changed", arg)
		self.state = node.ACTIVATED
		self.port_infos = arg

		self.adjust_children()
		

		
	def start(self):
		args = ["--library="+self.library, "--label="+self.label, "--service-name="+self.service_name]
		self.process.start("doob.ladspa", args)
		
		if False == self.process.waitForStarted():
			print ("Failed to start")
			return
		
		self.state = node.ALIVE
		


class midi_note_node(jack_client_node):
	def __init__(self):
		jack_client_node.__init__(self)


class graph_view(QGraphicsView):
	def to_json(self):
		t = self.transform()
		return {'__class__': 'graph_view', '__value__': [t.m11(), t.m12(), t.m13(), t.m21(), t.m22(), t.m23(), t.m31(), t.m32(), t.m33(), self.verticalScrollBar().value(), self.horizontalScrollBar().value(), self.zoom_factor_exponent ] } 
	
	
	def __init__(self, the_main_window):
		QGraphicsView.__init__(self)
		self.the_main_window = the_main_window
		
		self.zoom_factor_exponent = 0
		self.zoom_factor_base = 1.1
		self.setDragMode(QGraphicsView.RubberBandDrag)
		self.setRubberBandSelectionMode(Qt.IntersectsItemShape)
		self.setRenderHints(QPainter.Antialiasing)
		
		self.selected_start_port = None
		self.closest_port = None
		
	def overlay_timer_timeout(self):
		pass
	
	def mouseReleaseEvent(self, event):
		if event.button() == Qt.LeftButton:
			QGraphicsView.mouseReleaseEvent(self, event)
			
		if event.button() == Qt.RightButton:
			self.selected_start_port = None
			event.accept()
			
	def mousePressEvent(self, event):
		if event.button() == Qt.LeftButton:
			QGraphicsView.mousePressEvent(self, event)
		
		if event.button() == Qt.RightButton:
			self.selected_start_port = self.closest_port
			event.accept()
	
	def center_of_item(self, item):
		rect = item.boundingRect()
		pos = item.scenePos()
		return QPointF(pos.x() + rect.x() + 0.5 * rect.width(), pos.y() + rect.y() + 0.5 * rect.height())
	
	def distance(self, pos1, pos2):
		return math.sqrt(((pos1.x() - pos2.x()) ** 2.0) + ((pos1.y() - pos2.y()) ** 2.0))
	
	# Returns None if no port found
	def find_closest_port(self, pos):
		items = self.items()
		
		ports = filter(lambda x: x.data(node.IS_PORT_CONNECTOR).isValid(), items)
		
		if len(ports) > 0:
			min_distance = self.distance(self.center_of_item(ports[0]), pos)
			closest_port = ports[0]
			
			for index in range (1, len(ports)):
				d = self.distance(self.center_of_item(ports[index]), pos)
				if d < min_distance:
					min_distance = d
					closest_port = ports[index]
			
			# print min_distance
			return closest_port
		
		return None
		
	
	def mouseMoveEvent(self, event):
		QGraphicsView.mouseMoveEvent(self, event)

		pos = self.mapToScene(event.pos())
		
		# TODO: find a way to determine whether to draw the line or not
		#if not self.sceneRect().contains(pos):
		#	return
		
		self.closest_port = self.find_closest_port(pos)
		
		if self.closest_port:
			center_of_closest_port = self.center_of_item(self.closest_port)

			if self.selected_start_port:
				center_of_start_port = self.center_of_item(self.selected_start_port)
				self.the_main_window.potential_connection_line.setLine(center_of_start_port.x(), center_of_start_port.y(), center_of_closest_port.x(), center_of_closest_port.y())
			else:
				self.the_main_window.potential_connection_line.setLine(center_of_closest_port.x(), center_of_closest_port.y(), pos.x(), pos.y())

	
	def wheelEvent(self, event):
		if event.modifiers() & Qt.ControlModifier:
			if event.delta() > 0:
				self.zoom_factor_exponent += 1
			else:
				self.zoom_factor_exponent -= 1
				
			transform = self.transform()
			transform.setMatrix(1, 0, 0, 0, 1, 0, transform.m31(), transform.m32(), 1)
			scale = self.zoom_factor_base ** self.zoom_factor_exponent
			transform.scale(scale, scale)
			self.setTransform(transform)
			
			event.accept()
		else:
			QGraphicsView.wheelEvent(self, event)

class navigation_view(QGraphicsView):
	def __init__(self, the_main_window):
		QGraphicsView.__init__(self, the_main_window)
		
		self.the_main_window = the_main_window

		# TODO: fix this hack by proper event handling
		timer = QTimer(self)
		timer.setInterval(500)
		timer.setSingleShot(False)
		timer.timeout.connect(self.adjust)
		timer.start()
		

	def adjust(self):
		self.invalidateScene(layers = QGraphicsScene.ForegroundLayer)
		self.viewport().update()
		#self.update()
		pass
	
	def resizeEvent(self, event):
		QGraphicsView.resizeEvent(self, event)
		self.fit()
	
	def showEvent(self, event):
		QGraphicsView.showEvent(self, event)
		self.fit()
	
	def fit(self):
		self.fitInView(self.sceneRect(), Qt.KeepAspectRatio)
		# print "FIT"
		
	def drawForeground(self, painter, rect):
		QGraphicsView.drawForeground(self, painter, rect)
		if not self.the_main_window.tab_widget.currentIndex() == -1:
			painter.setBrush(QColor(255, 255, 255, 40))
			painter.setPen(QColor(255, 255, 255, 80))
			view = self.the_main_window.tab_widget.currentWidget()
			#painter.fillRect(view.mapToScene(view.viewport().geometry()).boundingRect(), QBrush(QColor(1.0, 1.0, 1.0, 0.5)))
			painter.drawRect(view.mapToScene(view.viewport().geometry()).boundingRect())
	
	def mousePressEvent(self, event):
		QGraphicsView.mousePressEvent(self, event)
		if not self.the_main_window.tab_widget.currentIndex() == -1:
			view = self.the_main_window.tab_widget.currentWidget()
			
			view.centerOn(self.mapToScene(event.pos()))
		