#ifndef DOOB_EVENT_HH
#define DOOB_EVENT_HH

#include <types.h>

namespace doob {

/**
 * Abstract base class for all events that can be stored in 
 * patterns
 */
struct event {
	virtual ~event() { }
};

} // namespace

#endif
