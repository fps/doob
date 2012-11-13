#ifndef DOOB_MIDI_TRACK_HH
#define DOOB_MIDI_TRACK_HH

#include <track.h>
#include <midi_events.h>
#include <midi_processor.h>

namespace doob {

/**
 * A midi track holds midi_events
 */
struct midi_track : track<midi_event>, midi_processor {
	midi_track(weak_ptr<engine> the_engine, string name, unsigned int num_channels) :
		midi_processor(the_engine, name, num_channels)
	{ }
	
	virtual bool is_rt() { return true; }
	virtual range span() { return span(); }
	virtual void render(tick_t tick, void* buffer) { }
};

typedef shared_ptr<midi_track> midi_track_ptr;

} // namespace

#endif
