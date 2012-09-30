#ifndef DOOB_STUFF_HH
#define DOOB_STUFF_HH

#include <types.h>

namespace doob {

struct stuff_base {
	virtual ~stuff_base() { }
};
	
/**
 * The base type for everything that can be
 * put on the heap and thus garbage collected.
 */
template<class T>
struct stuff {
	virtual ~stuff() { }
	
	protected:
	stuff() { }
	
	public:
	shared_ptr<T> create(T &t) {
		return shared_ptr<T>(new T(t));
	}
};

typedef shared_ptr<stuff_base> stuff_base_ptr;

} // namespace

#endif
