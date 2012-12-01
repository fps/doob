.PHONY: all install uninstall clean

PREFIX ?= /usr/local

CXX ?= g++
MOC ?= `pkg-config QtCore --variable=moc_location`

CXXFLAGS ?= -I . -g -O3 -ffast-math -mtune=native -Wall -fPIC `pkg-config --cflags QtCore QtDBus jack`
CFLAGS ?= $(CXXFLAGS)

LDFLAGS ?= `pkg-config --libs QtCore QtDBus jack` -lboost_program_options  -Wl,-rpath=$(PREFIX)/lib


DBUS_INTERFACES = ladspa jack_client
DBUS_SERVER_INTERFACE_XMLS =            $(DBUS_INTERFACES:%=dbus-introspection/%.xml)
DBUS_SERVER_INTERFACE_ADAPTORS =        $(DBUS_INTERFACES:%=doob/%_adaptor)
DBUS_SERVER_INTERFACE_ADAPTOR_SOURCES = $(DBUS_INTERFACES:%=doob/%_adaptor.cpp)
DBUS_SERVER_INTERFACE_ADAPTOR_HEADERS = $(DBUS_INTERFACES:%=doob/%_adaptor.h)

DBUS_SERVERS = ladspa midi_note

DBUS_SERVER_EXECUTABLES = $(DBUS_SERVERS:%=doob/doob.%)
DBUS_SERVER_HEADERS =     $(DBUS_SERVERS:%=doob/doob.%.h)
DBUS_SERVER_MOC_SOURCES = $(DBUS_SERVERS:%=doob/doob.%.moc.cc) 


LADSPA_DBUS_DEPENDENCIES = jack_client ladspa
MIDI_NOTE_DBUS_DEPENDENCIES = jack_client

.SECONDARY:

all: $(DBUS_SERVER_EXECUTABLES)
	@echo servers: $(DBUS_SERVER_EXECUTABLES)

doob/doob.ladspa: $(LADSPA_DBUS_DEPENDENCIES:%=doob/%_adaptor.o) $(LADSPA_DBUS_DEPENDENCIES:%=doob/%_adaptor.moc.o)
doob/doob.midi_note: $(MIDI_NOTE_DBUS_DEPENDENCIES:%=doob/%_adaptor.o) $(MIDI_NOTE_DBUS_DEPENDENCIES:%=doob/%_adaptor.moc.o)

doob/%_adaptor.h doob/%_adaptor.cpp: dbus-introspection/%.xml
	qdbusxml2cpp  -i "doob/doob.meta_types.h" -a doob/$*_adaptor $<

doob/%.moc.cc: doob/%.h
	$(MOC) -o $@ $<

doob/doob.%: doob/doob.%.o doob/doob.%.moc.o doob/doob.jack_client.o doob/doob.jack_client.moc.o  doob/doob.meta_types.o
	g++ -o $@ $(CXXFLAGS) $^  $(LDFLAGS)

clean:
	rm -rf $(DBUS_SERVER_EXECUTABLES) doob/*.o  $(DBUS_MOC_SOURCES) $(DBUS_SERVER_INTERFACE_ADAPTOR_SOURCES) $(DBUS_SERVER_INTERFACE_ADAPTOR_HEADERS)

install: all
	@echo TODO: install target
