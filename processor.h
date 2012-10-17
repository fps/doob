#ifndef DOOB_PROCESSOR_HH
#define DOOB_PROCESSOR_HH

#include <string>
#include <vector>

#include <types.h>
#include <midi_events.h>

namespace doob {

using std::string;
using std::vector;

struct engine;

/**
 * A processor is something that has
 * associated ports in the engine and 
 * is responsible for processing the 
 * data in these ports.
 * 
 * Each processor is run by the engine.
 */
struct processor {
	string name;
	
	vector<jack_port_t*> ports;
	
	vector<float*> in_port_buffers;
	vector<float*> out_port_buffers;
	
	weak_ptr<engine> the_engine;
	
	processor(
		weak_ptr<engine> engine,
		string name
	) :
		the_engine(engine),
		name(name)
	{ }
	
	virtual void process(jack_nframes_t nframes) = 0;

	virtual ~processor() { }
};

typedef shared_ptr<processor>  processor_ptr;



struct audio_processor : processor {

};

	
} // namespace

#endif
