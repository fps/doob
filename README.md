doob
====

doob booop deep 


Overview
========

doob consists of these main parts:

The GUI
-------

The GUI is mostly QGraphicsView based. There is one main
view of the song. This view is freely zoomable and
draggable. 

Sequence View
---------

Controls (incl. mouse):

* ctrl-scrollwheel - change the zoom factor. Note: zooming 
this way leaves the position under the mouse cursor invariant.

* ctrl-drag - move around in the view port


Controls (keyboard only)

* pgup/pgdown move up/down in the sequence view

* ctrl-pgup/pkdown - change the zoom factor. This way to 
zooom either leaves the center of the view invariant or,
if there's a current selection that is in the view, 
leave its position invariant.

Navigation View
---------------

This view always shows the complete song and the current
viewport as outlined rectangle. 

Controls:

* click to center viewport on the selected position

* ctrl-scrollwheel - change the zoom factor of the 
sequence view

* 



The Realtime Layer
------------------



The 
Threads that produce output that is not dependent on the
inputs. These threads do not use realtime scheduling. 

* Threads that consume output. These are subject to realtime 
constraints. 