#ifndef DOOB_TRACK_HH
#define DOOB_TRACK_HH

#include <pattern.h>
#include <types.h>
#include <time.h>
#include <range.h>

#include <map>

namespace doob {

using std::map;


struct track_base {
	virtual ~track_base() { }

	track_base()  { }
};

/**
 * A track holds patterns. There are several subtypes of tracks
 * for the different kinds of patterns.
 */
template<class EventType>
struct track : track_base {

	map<time, boost::shared_ptr<pattern<EventType> > > pattern_starts;
	map<time, boost::shared_ptr<pattern<EventType> > > pattern_ends;
	
	/**
	 * Subclasses must be able to determine their span.
	 * This function should be implemented as efficiently
	 * as possible as it is used by the song views to 
	 * determine what range to provide with e.g. the tick
	 * lines.
	 */
	virtual range span() = 0;
	
	/**
	 * A track can either be realtime safe or not.
	 * 
	 * If it's not, its output will be buffered
	 * in addition to the buffering of the jack
	 * backend.
	 * 
	 * Not being buffered makes changes in the 
	 * track to be audible more quickly
	 * than a buffered track.
	 */
	virtual bool is_rt() = 0;
	
	/**
	 * Asks those patterns to render whose
	 * ranges overlap with the tick. The 
	 * buffer is guaranteed to be large enough
	 * to contain all events/data for 
	 * the tick.
	 */
	virtual void render(range the_range, void* buffer) = 0;
};


} // namespace

#endif
