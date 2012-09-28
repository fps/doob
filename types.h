#ifndef DOOB_TYPES_HH
#define DOOB_TYPES_HH

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <jack/jack.h>

namespace doob {

using boost::shared_ptr;
using boost::weak_ptr;

typedef double tempo_t;

/**
 * A type for all times represented as ticks
 */
typedef jack_time_t tick_t;

} // namespace

#endif
