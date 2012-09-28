#ifndef DOOB_MIDI_SINK_HH
#define DOOB_MIDI_SINK_HH

#include <types.h>
#include <event.h>

#include <utility>

namespace doob {
	
using std::pair;

struct midi_sink_base {
	virtual ~midi_sink_base() { }
};

/**
* Abstract base type for all
* midi sinks.
* 
* the_callback has to be a functor 
* with a pair<time_t, midi_event> operator()()
* 
* It will be called to deliver midi 
* events. 
*/
template<class callback>
struct midi_sink : midi_sink_base {
	callback the_callback;
	
	midi_sink(callback the_callback) : 
		the_callback(the_callback) 
	{
		
	}
	
	/**
	* The minimum delay this sink has
	* before events fed to it take effect
	*/
	virtual time_t latency() = 0;
	
};

}
#endif
