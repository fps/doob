#include <singleton.h>
#include <song.h>

shared_ptr<singleton> singleton::instance = shared_ptr<singleton>();

singleton::singleton() : the_song(new song()) 
{ 
	
}
