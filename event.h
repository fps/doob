#ifndef DOOB_EVENT_HH
#define DOOB_EVENT_HH

#include <types.h>

namespace doob {

struct pattern;

/*
 * Abstract base class for all events that can be stored in 
 * patterns
 */
struct event {
	event(weak_ptr<pattern> owner) : owner(owner) { }
	
	virtual ~event() { }
	
	weak_ptr<pattern> owner;
};

struct midi_event {
	
};

}

#endif
