#ifndef DOOB_MIDI_EVENTS_HH
#define DOOB_MIDI_EVENTS_HH

#include <types.h>
#include <event.h>

namespace doob {

/**
 * Base class for all kinds of midi events
 */
struct midi_event : event {
	typedef unsigned int note_t;
	typedef unsigned int velocity_t;
	
	typedef unsigned int cc_number_t;
	typedef unsigned int cc_value_t;
};

typedef shared_ptr<midi_event> midi_event_ptr;

struct midi_note_on_event : midi_event {
	note_t note;
	velocity_t velocity;
};

struct midi_note_off_event : midi_event {
	note_t note;
	velocity_t velocity;
};

struct midi_cc_event : midi_event {
	cc_number_t number;
	cc_value_t value;
};

} // namespace

#endif

