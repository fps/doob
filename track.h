#ifndef DOOB_TRACK_HH
#define DOOB_TRACK_HH

#include <pattern.h>
#include <types.h>

#include <map>

using std::map;

namespace doob {

struct song;

/*
 * A track holds patterns. There are several subtypes of tracks
 * for the different kinds of patterns.
 * 
 * A track holds for each pattern the start end end positions.. This way
 * one can use the map find functions to locate all patterns that are 
 * "active" at a certain tick with O(log n) complexity.
 * 
 * Note that the start positions are inclusive, but the end
 * positions are exclusive
 */
struct track {
	virtual ~track() { }
	
	weak_ptr<song> owner;
	
	std::map<tick_size_t, boost::shared_ptr<pattern> > pattern_starts;
	std::map<tick_size_t, boost::shared_ptr<pattern> > pattern_ends;
};


/*
 * note_track is somewhat of a misnomer, since a note_track can
 * also hold control information
 */
struct note_track : track {
	
};

}

#endif
