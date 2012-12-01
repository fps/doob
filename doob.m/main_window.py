from PyQt4.QtGui import *
from PyQt4.QtCore import *

import subprocess

from graph import *

def to_json(python_object):
	if isinstance(python_object, QGraphicsScene):
		return {'__class__': 'QGraphicsScene', '__value__': { 'ladspa-nodes': filter(lambda x: isinstance(x, ladspa_node), python_object.items()) } }

	return python_object.to_json()

	
def from_json(the_main_window, json_dict):
	print json_dict
	if '__class__' in json_dict:
		c = json_dict["__class__"]
		v = json_dict["__value__"]
		if c == "graph_view":
			try:
				view = graph_view()
				view.setScene(the_main_window.scene)
				the_main_window.tab_widget.addTab(view, "View")
				transform = QTransform()
				transform.setMatrix(v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7], v[8])
				view.setTransform(transform)
				view.verticalScrollBar().setValue(v[9])
				view.horizontalScrollBar().setValue(v[10])
				view.zoom_factor_exponent = v[11]
			except:
				print "SAY WHAT?"
				pass
			
		if c == "ladspa_node":
			print "ladspa_node - "
			l = ladspa_node(v["library"], v["label"], v["uuid"])
			the_main_window.scene.addItem(l)

			l.setX(v["x"])
			l.setY(v["y"])
			
			return l
			
	return json_dict
	
	
class main_window(QMainWindow):
	def to_json(self):
		return {'__class__': 'main_window', '__value__': { 'scene': self.scene, 'views': [self.tab_widget.widget(index) for index in range(self.tab_widget.count())] } }

	def __init__(self, ladspa_plugins):
		QMainWindow.__init__(self)

		self.scene = QGraphicsScene()
		

		self.log = QTextEdit()
		self.log_dock = QDockWidget()
		self.log_dock.setObjectName("log_dock")
		self.log_dock.setWidget(self.log)
		self.log_dock.setWindowTitle("It's fresh, it's new, it's LOG!!!")
		self.addDockWidget(Qt.BottomDockWidgetArea, self.log_dock)

		self.nav_view = navigation_view(self)
		self.nav_view.setScene(self.scene)
		
		self.nav_dock = QDockWidget()
		self.nav_dock.setWidget(self.nav_view)
		self.nav_dock.setWindowTitle("Navigation")
		self.nav_dock.setObjectName("nav_dock")
		self.addDockWidget(Qt.RightDockWidgetArea, self.nav_dock)
		
		layout_widget = QWidget()
		box_layout = QVBoxLayout()
		
		self.plugin_selector_line_edit = QLineEdit()
		self.setup_plugin_selector(ladspa_plugins)
		self.plugin_selector_dock = QDockWidget()
		self.plugin_selector_dock.setObjectName("plugin_selector_dock")
		self.plugin_selector_dock.setWindowTitle("Plugin Selector")
		self.plugin_selector_dock.setWidget(self.plugin_selector_line_edit)
		self.addDockWidget(Qt.TopDockWidgetArea, self.plugin_selector_dock)
		# box_layout.addWidget(self.plugin_selector_line_edit)
		
		self.settings = QSettings("fps.io", "doob.m")
		if self.settings.contains("main_window/geometry"):
			self.restoreGeometry(self.settings.value("main_window/geometry").toByteArray())
		if self.settings.contains("main_window/state"):
			self.restoreState(self.settings.value("main_window/state").toByteArray())
			
		view1 = graph_view()
		view1.setScene(self.scene)
		
		self.tab_widget = QTabWidget()
		#self.tab_widget.addTab(view1, "View")

		box_layout.addWidget(self.tab_widget)
		
		layout_widget.setLayout(box_layout)
		
		self.setCentralWidget(layout_widget)
		
		self.setup_menu()

		self.scene.setBackgroundBrush(QBrush(QColor(48/2, 85/2, 67/2)));
		
		self.connection_update_timer = QTimer()
		self.connection_update_timer.setInterval(500)
		self.connection_update_timer.timeout.connect(self.connection_update_timeout)
		self.connection_update_timer.setSingleShot(False)
		self.connection_update_timer.start()

	def setup_menu(self):
		menubar = self.menuBar()
		
		file_menu = menubar.addMenu("&Serialization")
		
		save_action = QAction("&Serialize to /dev/stdout", self)
		save_action.setShortcut("Ctrl+S");
		file_menu.addAction(save_action)
		save_action.triggered.connect(self.save)
		
		view_menu = menubar.addMenu("&View")
		
		add_view_action = QAction("&Add view", self)
		add_view_action.setShortcut("Ctrl+A")
		view_menu.addAction(add_view_action)
		add_view_action.triggered.connect(self.add_view)

		remove_view_action = QAction("&Remove view", self)
		remove_view_action.setShortcut("Ctrl+R")
		view_menu.addAction(remove_view_action)
		remove_view_action.triggered.connect(self.remove_view)
		
		view_menu.addAction(self.log_dock.toggleViewAction())
		view_menu.addAction(self.nav_dock.toggleViewAction())
		view_menu.addAction(self.plugin_selector_dock.toggleViewAction())
		
		shrink_scene_action = QAction("&Shrink scene to item bounding rect", self)
		view_menu.addAction(shrink_scene_action)
		view_menu.triggered.connect(self. shrink_scene)
		
		
				
	def shrink_scene(self):
		self.nav_view.setSceneRect(self.scene.itemsBoundingRect())
		self.nav_view.fit()
		
	def add_view(self):
		view = graph_view()
		view.setScene(self.scene)
		self.tab_widget.addTab(view, "View")

	def remove_view(self):
		self.tab_widget.removeTab(self.tab_widget.currentIndex())

	def save(self):
		print "PSST:" + json.dumps(self, default=to_json)

	def setup_plugin_selector(self, ladspa_plugins):
		self.plugin_selector_line_edit.setPlaceholderText("Enter a plugin name here and press enter to add it to the graph")
		
		pluginlist = []
		for plugin in ladspa_plugins:
			pluginlist.append("LADSPA --- " + plugin[1] + " --- " + plugin[0] + " --- " + plugin[2])
		
		pluginlist.append("INTERNAL --- " + "MidiNote")
		
		completer = QCompleter(pluginlist)
		completer.setCaseSensitivity(Qt.CaseInsensitive)
		completer.setMaxVisibleItems(1000)

		self.plugin_selector_line_edit.setCompleter(completer)
		self.plugin_selector_line_edit.returnPressed.connect(self.plugin_selected)


	def closeEvent(self, event):
		self.settings.setValue("main_window/geometry", self.saveGeometry())
		self.settings.setValue("main_window/state", self.saveState())
		
		items = self.scene.items()
		for item in items:
			if type(item) == ladspa_node:
				item.stop()
				
		self.scene.clear()


	def plugin_selected(self):
		components = self.plugin_selector_line_edit.text().split(" --- ")
		print(list(components))
		
		if components[0] == "LADSPA":
			l = ladspa_node(components[2], components[1])
			self.scene.addItem(l)
			
		if components[0] == "INTERNAL":
			m = midi_note_node()
			self.scene.addItem(m)

	def log(self, text):
		self.log.append(text)

	def connection_update_timeout(self):
		jack_lsp_output = subprocess.check_output(["jack_lsp", "-c"])

		
		current_portname = ""
		jack_connections = set()
		for component in jack_lsp_output.split("\n"):
			if len(component) == 0:
				continue
			
			# print ("component", component)
			if component[0] == " ":
				jack_connections.add(frozenset([component.strip(), current_portname]))
			else: 
				current_portname = component.strip()
			# print component
			
		print jack_connections