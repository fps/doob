import sys
import os
import subprocess
import string
import json

import dbus

from PyQt4.QtGui import *
from PyQt4.QtCore import *
from dbus.mainloop.qt import DBusQtMainLoop

from graph import *
from main_window import *
from ladspa import *




app = QApplication(sys.argv)

dbus.set_default_main_loop(dbus.mainloop.qt.DBusQtMainLoop())

the_main_window = main_window()
the_main_window.show()

if False == os.isatty(0):
	the_line = ""
	for line in sys.stdin:
		if string.rfind(line, "PSST:") == 0:
			the_line = line[len("PSST:"):]
	
	if len(the_line) > 0:
		#last line has to be most current state
		json.loads(the_line, object_hook=lambda x: from_json(the_main_window, x))

if the_main_window.tab_widget.count() == 0:
	the_main_window.add_view()

the_main_window.post_show()

sys.exit(app.exec_())
