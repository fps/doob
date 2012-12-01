#include <doob/doob.meta_types.h>

#include <QDBusArgument>
#include <QDBusMetaType>

QDBusArgument &operator<<(QDBusArgument &argument, const LadspaPort &mystruct) {
	argument.beginStructure();
	argument << mystruct.name;
	argument << mystruct.descriptor;
	argument << mystruct.hints;
	argument << mystruct.jack_name;
	argument.endStructure();
	return argument;
}

// Retrieve the MyStructure data from the D-Bus argument
const QDBusArgument &operator>>(const QDBusArgument &argument, LadspaPort &mystruct) {
	argument.beginStructure();
	argument >> mystruct.name;
	argument >> mystruct.descriptor;
	argument >> mystruct.hints;
	argument >> mystruct.jack_name;
	argument.endStructure();
	return argument;
}


QDBusArgument &operator<<(QDBusArgument &argument, const JackPort &mystruct) {
	argument.beginStructure();
	argument << mystruct.name;
	argument << mystruct.is_input;
	argument << mystruct.is_output;
	argument << mystruct.is_audio;
	argument << mystruct.is_midi;
	argument.endStructure();
	return argument;
}

// Retrieve the MyStructure data from the D-Bus argument
const QDBusArgument &operator>>(const QDBusArgument &argument, JackPort &mystruct) {
	argument.beginStructure();
	argument >> mystruct.name;
	argument >> mystruct.is_input;
	argument >> mystruct.is_output;
	argument >> mystruct.is_audio;
	argument >> mystruct.is_midi;
	argument.endStructure();
	return argument;
}

void register_meta_types() {
	qDBusRegisterMetaType<LadspaPort>();
	qDBusRegisterMetaType<LadspaPortList>();
	qDBusRegisterMetaType<JackPort>();
	qDBusRegisterMetaType<JackPortList>();
}