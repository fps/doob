#ifndef DOOB_MIDI_PROCESSOR_HH
#define DOOB_MIDI_PROCESSOR_HH

#include <types.h>

#include <processor.h>
#include <engine.h>
#include <assign.h>
#include <dbg.h>

#include <string>

namespace doob {
	
using std::string;

struct midi_processor : processor {
	midi_processor(weak_ptr<engine> the_engine, string name, unsigned int num_channels) :
		processor(the_engine, name)
	{ 
		for (
			unsigned int index = 0; 
			index < num_channels; 
			++index) 
		{	
			stringstream stream;
			stream << name << "_in_" << index;
			
			jack_port_t *port = jack_port_register(
				the_engine.lock()->jack_client,
				stream.str().c_str(),
				JACK_DEFAULT_MIDI_TYPE,
				JackPortIsInput,
				0
			);
		
			if (NULL == port) {
				throw runtime_error("could not register midi port " + stream.str());
			}
			
			ports.push_back(port);

			stringstream stream2;
			stream2 << name << "_out_" << index;
			
			port = jack_port_register(
				the_engine.lock()->jack_client,
				stream2.str().c_str(),
				JACK_DEFAULT_MIDI_TYPE,
				JackPortIsOutput,
				0
			);
		
			if (NULL == port) {
				throw runtime_error("could not register midi port " + stream.str());
			}
			
			ports.push_back(port);
		}
	}
	
	virtual void process(jack_nframes_t nframes) { DBG(the_engine.lock()->jack_client) }
};

typedef shared_ptr<midi_processor> midi_processor_ptr;

} // namespace

#endif