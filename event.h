#ifndef DOOB_EVENT_HH
#define DOOB_EVENT_HH

#include <types.h>

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

#endif
