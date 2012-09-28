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
	range loop_range;
	
	vector<shared_ptr<track> > tracks;
	
	song() {
		
	}
};

}

#endif
