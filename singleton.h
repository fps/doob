#ifndef DOOB_SINGLETON_HH
#define DOOB_SINGLETON_HH

#include <types.h>

struct song;

/*
 * The application wide singleton
 */
struct singleton {
	protected:
	singleton();
	
	static shared_ptr<singleton> instance;
	
	public:
	static shared_ptr<singleton> get() {
		if (false == instance) {
			instance = shared_ptr<singleton>(new singleton());
		}
		return instance;
	}

	void remove() {
		instance = shared_ptr<singleton>();
	}
	
	shared_ptr<song> the_song;
};

#endif
