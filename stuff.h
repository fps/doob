#ifndef DOOB_STUFF_HH
#define DOOB_STUFF_HH

#include <types.h>

namespace doob {

/**
 * The base type for everything that can be
 * put on the heap and thus garbage collected.
 */
struct stuff {
	virtual ~stuff() { }
};

typedef shared_ptr<stuff> stuff_ptr;

} // namespace

#endif
