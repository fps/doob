#ifndef DOOB_PATTERN_HH
#define DOOB_PATTERN_HH

#include <types.h>

// #include <vector>
//#include <map>

namespace doob {

//using std::vector;
//using std::map;

struct track;

/*
 * Abstract Base Class for all kinds of patterns
 */
struct pattern {
	pattern(weak_ptr<track> owner) : owner(owner) { }
	
	virtual ~pattern() { }
	
	virtual tick_t length() = 0;
	
	weak_ptr<track> owner;
};

struct pattern_group : pattern {
	//vector<shared_ptr<pattern> > children;
};

struct note_pattern {
	//map<tick_t, note_ptr> notes;
};

} // namespace

#endif
