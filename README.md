# doob

doob booop deep 

A somewhat different take on the sequencer/tracker thing.. 
Highly experimental. Will crash and burn..

NOTE: Nothing of the below is implemented yet. This text
simply serves as drawing board for ideas

# Overview

doob consists of these main parts:

## The GUI

The GUI is mostly QGraphicsView based. There is one main
view of the song. This view is freely zoomable and
draggable. 

### Sequence View

Global sequence view controls (incl. mouse):

* ctrl-scrollwheel - change the zoom factor. Note: zooming 
this way leaves the position under the mouse cursor invariant.

* ctrl-drag - move around in the view port

* scrollwheel - scroll up and down in the viewport

* shift-scrollwheel - scroll sideways in the viewport

* horizontal scrollwheel - scroll sideways in the viewport

Global sequence view controls (keyboard only)

* pgup/pgdown move up/down in the sequence view

* ctrl-pgup/pkdown - change the zoom factor. This way to 
zooom either leaves the center of the view invariant or,
if there's a current selection that is in the view, 
leave its position invariant.

#### Tracks

The sequence view contains different tracks.

##### 

### Navigation View

This view always shows the complete song and the current
viewport as outlined rectangle. 

Controls:

* click to center viewport on the selected position

* ctrl-scrollwheel - change the zoom factor of the 
sequence view


## Realtime Considerations

Since doob allows almost arbitrary scripting languages be 
integrated to generate patterns (right now we only 
support Lua) patterns can be either marked as realtime
or non-realtime, since Lua snippets to produce e.g.
midi sequences are inherintly non-realtime, non-realtime
patterns operate on much larger buffer sizes to allow
at least soft-realtime operation.

### Realtime Patterns


