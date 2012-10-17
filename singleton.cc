#include <singleton.h>
#include <song.h>

namespace doob {

shared_ptr<singleton> singleton::instance = shared_ptr<singleton>();

singleton::singleton() : 
	the_engine(new engine()),
	the_song(new song()),
	the_heap(new heap())
{ 
	
}

} // namespace