#ifndef DOOB_TYPES_HH
#define DOOB_TYPES_HH

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <jack/jack.h>

namespace doob {

using boost::shared_ptr;
using boost::weak_ptr;

typedef double tempo_t;

typedef jack_time_t audio_frame_t;

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
	
	audio_frame_t to_frametime(double samplerate) {
		return (t * (time_t) samplerate) / base;
	}
#if 0
	time &operator=(frame_t frame) {
		
	}
#endif
};

} // namespace

#endif
