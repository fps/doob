#ifndef DOOB_TYPES_HH
#define DOOB_TYPES_HH

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <jack/jack.h>

namespace doob {
	
	using boost::shared_ptr;
	using boost::weak_ptr;

	typedef long long tick_size_t;
	typedef double tempo_t;
	typedef jack_time_t time_t;
}

#endif
