#ifndef DOOB_NOTE_HH
#define DOOB_NOTE_HH

#include <types.h>

typedef double note_t;
typedef double velocity_t;

struct note {
	virtual ~note() { }
};

typedef shared_ptr<note> note_ptr;

struct note_on : note {
	note_t note;
	velocity_t velocity;
};

struct note_off : note {
	velocity_t velocity;
};

#endif
