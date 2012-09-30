#ifndef DOOB_TRACK_HH
#define DOOB_TRACK_HH

#include <pattern.h>
#include <types.h>
#include <range.h>

#include <map>

namespace doob {

using std::map;

struct song;

struct track_base {
	virtual ~track_base() { }

	track_base(weak_ptr<song> owner) : owner(owner) { }
	
	weak_ptr<song> owner;
};

/**
 * A track holds patterns. There are several subtypes of tracks
 * for the different kinds of patterns.
 */
template<class EventType>
struct track : track_base {

	map<tick_t, boost::shared_ptr<pattern<EventType> > > pattern_starts;
	map<tick_t, boost::shared_ptr<pattern<EventType> > > pattern_ends;
	
	/**
	 * Subclasses must be able to determine their span.
	 * This function should be implemented as efficiently
	 * as possible as it is used by the song views to 
	 * determine what range to provide with e.g. the tick
	 * lines.
	 */
	virtual range span() = 0;
	
	// virtual void render
};


} // namespace

#endif
