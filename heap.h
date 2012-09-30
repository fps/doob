#ifndef DOOB_HEAP_HH
#define DOOB_HEAP_HH

#include <stuff.h>

#include <list>

namespace doob {

using std::list;

/**
 * A poor man's garbage collection mechanism
 */
struct heap {
	list<stuff_base_ptr> the_stuff;
	
	/**
	 * call this function regularly to 
	 * remove deserted objects from the 
	 * heap. 
	 * 
	 * NOTE: This function should be 
	 * called regularly from the GUI
	 * thread.
	 */
	void cleanup() {
		for (
			list<stuff_base_ptr>::iterator it = the_stuff.begin();
			it != the_stuff.end();
		) {
			if (it->unique()) {
				/**
				 * erase() gives back an
				 * iterator advanced to the
				 * following element
				 */
				it = the_stuff.erase(it);
			} else {
				/**
				 * In this case just advance
				 * the iterator
				 */
				++it;
			}
		}
			
	}
	
	/**
	 * Add some stuff to the heap. This
	 * way a reference is held.
	 * 
	 * NOTE: This function should be
	 * called only by the same thread
	 * that calls cleanup(), too.
	 */
	void add(stuff_base_ptr some_stuff) {
		the_stuff.push_back(some_stuff);
	}
};

} // namespace
#endif
