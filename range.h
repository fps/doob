#ifndef DOOB_RANGE_HH
#define DOOB_RANGE_HH

#include <types.h>

struct range {
	/*
	 * The start tick is included i.e. start <= range
	 */
	tick_size_t start;
	
	/*
	 * The end tick is excluded range < end
	 */
	tick_size_t end;
};

#endif
