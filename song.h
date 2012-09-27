#ifndef DOOB_SONG_HH
#define DOOB_SONG_HH

#include <types.h>
#include <range.h>
#include <track.h>

#include <vector>

using std::vector;

struct song {
	/*
	 * A song consists of 
	 * 
	 * * Tracks
	 * 
	 * * Instruments
	 * 
	 * and some global parameters
	 */
	range loop_range;
	
	vector<shared_ptr<track> > tracks;
};

#endif
