#ifndef DOOB_PATTERN_HH
#define DOOB_PATTERN_HH

#include <types.h>
#include <midi_events.h>
#include <range.h>

#include <vector>
#include <map>


namespace doob {

using std::vector;
using std::map;

template<class EventType>
struct track;

/*
 * Abstract Base Class for all kinds of patterns
 */
template<class EventType>
struct pattern {
	pattern(weak_ptr<track<EventType> > owner) : owner(owner) { }
	
	virtual ~pattern() { }
	
	virtual range span() const = 0;
	
	/**
	 * If a pattern reports true == is_realtime(), then
	 * its render() function has to meet realtime 
	 * constraints
	 */
	virtual bool is_realtime() const = 0;
	
	weak_ptr<track<EventType> > owner;
};

template<class EventType>
struct pattern_group : pattern<EventType> {
	vector<shared_ptr<pattern<EventType> > > children;
};

template<class EventType>
struct midi_pattern : pattern<EventType> {
	map<time, midi_event_ptr> notes;
};

} // namespace

#endif
