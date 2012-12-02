from PyQt4.QtCore import *
from PyQt4.QtGui import *

from graph import *

class plugin_inspector_model(QAbstractItemModel):
	def __init__(self, scene, parent = None):
		QAbstractItemModel.__init__(self, parent)
		self.scene = scene
		self.scene.changed.connect(self.selection_changed)
		self.selected_items = []
		
		self.setHeaderData(0, Qt.Horizontal, QVariant("Plugin"), Qt.DisplayRole)
		self.setHeaderData(0, Qt.Horizontal, QVariant("Plugin"), Qt.EditRole)
		self.headerDataChanged.emit(Qt.Horizontal, 0, 0)
		
	def selection_changed(self):
		self.selected_items = filter(lambda x:isinstance(x, ladspa_node), self.scene.selectedItems())
		if len(self.selected_items) == 0:
			self.selected_items = filter(lambda x:isinstance(x, ladspa_node), self.scene.items())
		self.modelReset.emit()
		pass

	def parent(self, index):
		return QModelIndex()
	
	def index(self, row, column, parent):
		return self.createIndex(row, column, self.selected_items[row])
	
	def columnCount(self, index):
		return 1
	
	def rowCount(self, index):
		# if top of the tree:
		if False == index.isValid():
			return len(self.selected_items)
		
		return 0
	
	def data(self, index, role):
		if role == Qt.DisplayRole:
			return QVariant(self.selected_items[index.row()].plugin_name.toPlainText())
		return QVariant()
