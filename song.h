#ifndef DOOB_SONG_HH
#define DOOB_SONG_HH

#include <types.h>
#include <range.h>
#include <track.h>

#include <vector>

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
 */
struct song {
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
	
	vector<shared_ptr<track> > tracks;
	
	song() {
		
	}
};

} // namespace

#endif
