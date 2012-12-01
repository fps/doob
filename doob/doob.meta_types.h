#ifndef POOQ_METATYPES_HH
#define POOQ_METATYPES_HH

#include <QMetaType>

#include <QList>
#include <QPair>
#include <QString>

struct JackPort {
	QString name;
	int is_input;
	int is_output;
	int is_audio;
	int is_midi;
	
	JackPort(QString name = "", int is_input = 0, int is_output = 0, int is_audio = 0, int is_midi = 0) :
		name(name),
		is_input(is_input),
		is_output(is_output),
		is_audio(is_audio),
		is_midi(is_midi)
	{ }
};

typedef QList<JackPort> JackPortList;

Q_DECLARE_METATYPE(JackPort)
Q_DECLARE_METATYPE(JackPortList)

struct LadspaPort {
	QString name;
	int descriptor;
	int hints;
	QString jack_name;

	LadspaPort(QString name = "", int descriptor = 0, int hints = 0, QString jack_name = "") :
		name(name),
		descriptor(descriptor),
		hints(hints),
		jack_name(jack_name)
	{ }
};

typedef QList<LadspaPort> LadspaPortList;

Q_DECLARE_METATYPE(LadspaPort)
Q_DECLARE_METATYPE(LadspaPortList)

void register_meta_types();

#endif

