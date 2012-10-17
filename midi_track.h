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

};

} // namespace

#endif
