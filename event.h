#ifndef DOOB_EVENT_HH
#define DOOB_EVENT_HH

#include <types.h>

namespace doob {

struct pattern;

/**
 * Abstract base class for all events that can be stored in 
 * patterns
 */
struct event {
	event(weak_ptr<pattern> owner) : owner(owner) { }
	
	virtual ~event() { }
	
	weak_ptr<pattern> owner;
};


/**
 * Base class for all kinds of events
 */
struct midi_event : event {
	typedef unsigned int note_t;
	typedef unsigned int velocity_t;
	
	typedef unsigned int cc_number_t;
	typedef unsigned int cc_value_t;
};

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
