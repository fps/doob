#ifndef DOOB_TRACK_HH
#define DOOB_TRACK_HH

#include <pattern.h>
#include <types.h>
#include <range.h>

#include <map>

namespace doob {

using std::map;

struct song;

/**
 * A track holds patterns. There are several subtypes of tracks
 * for the different kinds of patterns.
 * 
 * A track holds for each pattern the start end end positions.. This way
 * one can use the map find functions to locate all patterns that are 
 * "active" at a certain tick with O(log n) complexity.
 * 
 * Note that the start positions are inclusive, but the end
 * positions are exclusive
 * 
 * Note also that there is no notion of when a track starts or ends. 
 * Patterns can have arbitrary start and end positions
 */
struct track {
	virtual ~track() { }
	
	track(weak_ptr<song> owner) : owner(owner) { }
	
	weak_ptr<song> owner;
	
	map<tick_t, boost::shared_ptr<pattern> > pattern_starts;
	map<tick_t, boost::shared_ptr<pattern> > pattern_ends;

	/**
	 * Subclasses must be able to determine their span.
	 * This function should be implemented as efficiently
	 * as possible as it is used by the song views to 
	 * determine what range to provide with e.g. the tick
	 * lines.
	 */
	virtual range span() { 
		return range();
	}
};

/**
 * A midi track holds midi_events
 */
struct midi_track : track {
	
};

} // namespace

#endif
