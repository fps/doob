#ifndef DOOB_PROCESSOR_HH
#define DOOB_PROCESSOR_HH

#include <string>
#include <vector>

#include <types.h>
#include <midi_events.h>

namespace doob {

using std::string;
using std::vector;
	
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
	
	unsigned int num_in_ports;
	unsigned int num_out_ports;
	
	processor(
		string name, 
		unsigned int num_in_ports,
		unsigned int num_out_ports) 
	: 
		name(name),
		num_in_ports(num_in_ports),
		num_out_ports(num_out_ports)
	{ }
	
	/**
	 * See the jack documentation for the 
	 * interpretation of the void * data.
	 * 
	 * For every in and out port, there's 
	 * an entry in the argument vectors.
	 */
	virtual void process(vector<void*> &ins, vector<void*> &outs) = 0;

	virtual ~processor() { }
};

typedef shared_ptr<processor>  processor_ptr;

struct midi_processor : processor {
	/**
	 * See the jack documentation for the 
	 * interpretation of the void * data.
	 * 
	 * For every in and out port, there's 
	 * an entry in the argument vectors.
	 */
	virtual void process(vector<void*> &ins, vector<void*> &outs) = 0;
};

struct audio_processor : processor {
	/**
	 * See the jack documentation for the 
	 * interpretation of the void * data.
	 * 
	 * For every in and out port, there's 
	 * an entry in the argument vectors.
	 */
	virtual void process(vector<float*> &ins, vector<float*> &outs) = 0;
};

	
} // namespace

#endif
