from PyQt4.QtCore import *
from PyQt4.QtGui import *
import sys

class hacky_proxy_model(QSortFilterProxyModel):
	def __init__(self, parent):
		QSortFilterProxyModel.__init__(self, parent)
		self.s = ""
	
	def set_substring(self, s):
		self.s = s
		model = self.sourceModel()
		model.modelReset.emit()
	
	def filterAcceptsRow(self, sourceRow, sourceParent):
		index0 = self.sourceModel().index(sourceRow, 0, sourceParent)
		return self.s in self.sourceModel().data(index0, Qt.DisplayRole).toString().toLower()

class plugin_completer(QCompleter):
	def __init__(self, parent):
		QCompleter.__init__(self, parent)
		
	def setModel(self, model):
		self.proxy_model = hacky_proxy_model(self)
		self.proxy_model.setSourceModel(model)
		QCompleter.setModel(self, self.proxy_model)

	def splitPath(self, path):
		self.proxy_model.set_substring(path)
		return ""


class plugin_model(QAbstractItemModel):
	def __init__(self, parent, ladspa_plugins):
		QAbstractItemModel.__init__(self, parent)
		self.ladspa_plugins = ladspa_plugins
	
	def parent(self, index):
		return QModelIndex()
	
	def index(self, row, column, parent):
		return self.createIndex(row, column, self.ladspa_plugins[row])
	
	def columnCount(self, index):
		return 1
	
	def rowCount(self, index):
		return len(self.ladspa_plugins)
	
	def data(self, index, role):
		plugin = self.ladspa_plugins[index.row()]
		if role == Qt.DisplayRole:
			return QVariant(plugin['name'] + " ~~~ " + plugin['label'] + " ~~~ " + plugin['library'] + " ~~~ LADSPA")
		return QVariant()
