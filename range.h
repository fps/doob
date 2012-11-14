#ifndef DOOB_RANGE_HH
#define DOOB_RANGE_HH

#include <types.h>
#include <time.h>

namespace doob {

struct range {
	range(time start = time(), time end = time()) :
		start(start),
		end(end)
	{
		
	}
	
	/**
	 * The start tick is included i.e. start <= range
	 */
	time start;
	
	/**
	 * The end tick is excluded range < end
	 */
	time end;
};

} // namespace

#endif
