#ifndef DOOB_SONG_HH
#define DOOB_SONG_HH

#include <types.h>
#include <range.h>
#include <track.h>
#include <stuff.h>

#include <vector>

#include <QObject>

namespace doob {

using std::vector;

/**
 * A song consists of 
 * 
 * * Tracks
 * 
 * * Instruments
 * 
 * and some global parameters
 * 
 * NOTE: all GUI edit operations
 * have to pass through a ringbuffer 
 * to be performed by the realtime
 * backend.
 * 
 * Once the edit is complete a number of
 * signals will be emitted (depending
 * on what changed), so views can update 
 * their state..
 * 
 * NOTE: All objects added to a song
 * have to be also registered with 
 * the heap, so safe disposable in a
 * non-realtime thread can happen
 * once there's no more reference to
 * it. 
 */
struct song : QObject, stuff_base {
	Q_OBJECT
	
	public:
	typedef double tempo_t;
	
	/**
	 * The (base) tempo of the song is measured in
	 * beats per minute (bpm)
	 */
	tempo_t tempo;
	
	/**
	 * The global loop range. Note that patterns
	 * can have their own loop range
	 */
	range loop_range;
	
	shared_ptr<vector<shared_ptr<track_base> > > tracks;
	
	song() {
		
	}
};

typedef shared_ptr<song> song_ptr;

} // namespace

#endif
