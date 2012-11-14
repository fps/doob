#ifndef DOOB_TIME_HH
#define DOOB_TIME_HH

#include <types.h>

namespace doob {

struct time {
	time() : t(0) { } 
	
	typedef long long int time_t;
	/**
	 * The time..
	 */
	time_t t;

	/**
	 * What fraction of a second is
	 * represented by one tick
	*/
	static long long int base;
	
	frame_t to_frametime(double samplerate) {
		return (t * (time_t) samplerate) / base;
	}
#if 0
	time &operator=(frame_t frame) {
		
	}
#endif
};

} // namespace

#endif
