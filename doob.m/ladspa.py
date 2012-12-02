import os
import sys
import subprocess

# callback can be a callable that takes
# text as arguments
def discover_ladspa_plugins(callback = lambda x: x):
	print "Discovering LADSPA plugins"
	# TODO: more robust discovery?
	ladspa_plugins = [ ]
	ladspa_path = "/usr/lib/ladspa"
	if "LADSPA_PATH" in os.environ:
		ladspa_path = os.environ["LADSPA_PATH"]
		
	for path in ladspa_path.split(":"):
		# print ("LADSPA_PATH component: ", path)
		try:
			files = [ f for f in os.listdir(path) ]
			for f in files:
				lib = path + "/" + f
				
				try:
					library_text = subprocess.check_output(["analyseplugin", lib])
				except subprocess.CalledProcessError:
					print ("Something went wrong analysing", lib)
					continue
				
				label_lines = [l for l in library_text.split('\n') if "Plugin Label:" in l]
				name_lines = [l for l in library_text.split('\n') if "Plugin Name:" in l]
				for label_line, name_line in zip(label_lines, name_lines):
					label =  label_line[len("Plugin Label: \""):-1]
					name =  name_line[len("Plugin Name: \""):-1]

					ladspa_plugins.append({ 'library': lib, 'label': label, 'name': name})
					
					if None != callback:
						callback("LADSPA plugin: " + name + " " + label + " " + lib)
		except OSError:
			if None != callback:
				callback("Problem with: " + path)
	
	return ladspa_plugins

