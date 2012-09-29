#ifndef DOOB_SINK_HH
#define DOOB_SINK_HH

namespace doob {

/**
 * A sink is something that that consumes data.
 * The data is provided by a callback.
 * 
 * This callback has to meet realtime constraints.
 */
struct sink {
	virtual ~sink() { }
};

struct buffered_sink {
	
};

} // namespace

#endif
