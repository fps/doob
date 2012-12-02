from PyQt4.QtCore import *
from PyQt4.QtGui import *

from graph import *

class plugin_inspector_model(QAbstractItemModel):
	def __init__(self, scene, parent = None):
		QAbstractItemModel.__init__(self, parent)
		self.scene = scene
		self.scene.selectionChanged.connect(self.selection_changed)
		self.selected_items = []
		
	def headerData(self, section, orientation, role):
		if not role == Qt.DisplayRole:
			return QVariant()
		
		if 0 == section:
			return QVariant("Plugin")
		if 1 == section:
			return QVariant("Value")
		
		return QVariant()
	
	def selection_changed(self):
		self.selected_items = filter(lambda x:isinstance(x, ladspa_node), self.scene.selectedItems())
		
		if len(self.selected_items) == 0:
			self.selected_items = filter(lambda x:isinstance(x, ladspa_node), self.scene.items())
		
		self.modelReset.emit()


	def parent(self, index):
		if isinstance(index.internalPointer(), ladspa_node):
			return QModelIndex()
		
		for row in range(len(self.selected_items)):
			if self.selected_items[row] == index.internalPointer().parentItem():
				return self.createIndex(row, 0, self.selected_items[row])
	
	def index(self, row, column, parent):
		if not parent.isValid():
			return self.createIndex(row, column, self.selected_items[row])
		
		if isinstance(parent.internalPointer(), ladspa_node):
			if 0 == column:
				return self.createIndex(row, column, parent.internalPointer().sink_ports[row])
			if 1 == column:
				return self.createIndex(row, column, parent.internalPointer().sink_ports[row])
		
		return QModelIndex()
	
	def columnCount(self, parent):
		return 2
	
	def rowCount(self, parent):
		# if top of the tree:
		if not parent.isValid():
			return len(self.selected_items)
		
		if isinstance(parent.internalPointer(), ladspa_node):
			return len(parent.internalPointer().sink_ports)
		
		return 0
	
	def data(self, index, role):
		if not role == Qt.DisplayRole:
			return QVariant()

		if isinstance(index.internalPointer(), ladspa_node):
			if 0 == index.column():
				return QVariant(self.selected_items[index.row()].plugin_name.toPlainText())
			else:
				return QVariant()
		
		if 0 == index.column():
			return QVariant(index.internalPointer().toPlainText())
		if 1 == index.column():
			return QVariant(0.0)
		
		return QVariant()
	

