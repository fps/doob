#ifndef DOOB_SINGLETON_HH
#define DOOB_SINGLETON_HH

#include <types.h>
#include <dbg.h>
#include <heap.h>

namespace doob {

struct song;

/**
 * The application wide singleton
 */
struct singleton {
	protected:
	singleton();
	
	static shared_ptr<singleton> instance;
	
	public:
	~singleton() {
		DBG("singleton destroyed")
	}
	
	/**
	 * If necessary this function creates 
	 * the singleton instance
	 */
	static shared_ptr<singleton> get() {
		if (false == instance) {
			instance = shared_ptr<singleton>(new singleton());
		}
		
		return instance;
	}

	/**
	 * Note that remove might not delete the instance
	 * if other copies of the shared_ptr instance
	 * are still alive, but you get a new instance
	 * the next time you call get()
	 */
	void remove() {
		instance = shared_ptr<singleton>();
	}
	
	shared_ptr<song> the_song;
	heap the_heap;
};

typedef shared_ptr<singleton> singleton_ptr;

} // namespace

#endif
