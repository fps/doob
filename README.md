# doob

A collection of loosely coupled components communicating over DBUS that together can be used to create music software. One example is doob.m a graphical frontend.

# Requirements

python >= 2.7
python-dbus
PyQt4

libqt4-dev (with DBUS support)
qdbusxml2cpp

# Building

Type 

<pre>
make -j2
</pre>

to build the project. You can change the -j4 to any -jN where N is the number of parallel make jobs.

# doob.m

A graphical frontend to the different components implementing a modular synth.

# pooq.ladspa

A simple LADSPA host app exposing control and audio ports as jack ports.

# pooq.midi_note

A simple jack app that translates jack_midi events into audio rate signals.

