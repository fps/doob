import sys
import os
import subprocess
import string
import json

from PyQt4.QtGui import *
from PyQt4.QtCore import *
from dbus.mainloop.qt import DBusQtMainLoop

from graph import *
from main_window import *
import dbus


def discover_ladspa_plugins():
	# TODO: more robust discovery?
	ladspa_plugins = []
	ladspa_path = "/usr/lib/ladspa"
	if "LADSPA_PATH" in os.environ:
		ladspa_path = os.environ["LADSPA_PATH"]
		
	for path in ladspa_path.split(":"):
		try:
			print ("LADSPA_PATH component: ", path)
			files = [ f for f in os.listdir(path) ]
			for f in files:
				print ("LADSPA lib:", f)
				try:
					library_text = subprocess.check_output(["analyseplugin", f])
					label_lines = [l for l in library_text.split('\n') if "Plugin Label:" in l]
					name_lines = [l for l in library_text.split('\n') if "Plugin Name:" in l]
					for label_line, name_line in zip(label_lines, name_lines):
						label =  label_line[len("Plugin Label: \""):-1]
						name =  name_line[len("Plugin Name: \""):-1]
						ladspa_plugins.append([path + "/" + f, label, name])
				except:
					pass
		except:
			pass
	
	return ladspa_plugins


app = QApplication(sys.argv)

dbus.set_default_main_loop(dbus.mainloop.qt.DBusQtMainLoop())

the_main_window = main_window(discover_ladspa_plugins())
the_main_window.show()

if False == os.isatty(0):
	the_line = ""
	for line in sys.stdin:
		if string.rfind(line, "PSST:") == 0:
			the_line = line[len("PSST:"):]
	
	if len(the_line) > 0:
		#last line has to be most current state
		json.loads(the_line, object_hook=lambda x: from_json(the_main_window, x))

sys.exit(app.exec_())
