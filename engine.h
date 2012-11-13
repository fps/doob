#ifndef DOOB_ENGINE_HH
#define DOOB_ENGINE_HH

#include <dbg.h>
#include <processor.h>
#include <ringbuffer.h>

#include <boost/function.hpp>

#include <jack/jack.h>
#include <jack/midiport.h>

#include <vector>
#include <list>
#include <string>
#include <stdexcept>
#include <sstream>

namespace doob {
	struct engine;
}

extern "C" { 
	int doob_engine_process(jack_nframes_t nframes, void *arg);
}


namespace doob {

using std::vector;
using std::list;
using std::string;
using std::runtime_error;
using std::stringstream;

typedef boost::function<void(void)> command_t;

/**
 * The engine consumes midi_events and audio from tracks
 * (ultimately from patterns) and plays them back via
 * jack.
 */
struct engine {
	/**
	 * commands are functors to be executed
	 * in the RT context. 
	 */
	ringbuffer<command_t> commands;
	
	/**
	 * Replies sent by the RT context.
	 */
	ringbuffer<command_t> replies;
	
	jack_client_t *jack_client;
	
	typedef list<processor_ptr> processors_t;
	typedef shared_ptr<processors_t> processors_t_ptr;
	
	processors_t_ptr processors;
	
	public:
	/**
	 * NOTE: this constructor might throw.
	 * Among others it might throw std::runtime_errors
	 * which give a textual representation of the
	 * reason for the failure..
	 */
	engine(
		
	) :
		commands(1024),
		replies(1024),
		processors(processors_t_ptr(new processors_t))
	{
		DBG("engine()")
		
		jack_client = jack_client_open("doob", JackNullOption, NULL, 0);
		
		if (NULL == jack_client) {
			throw runtime_error("Could not register jack client");
		}
				
		jack_set_process_callback(jack_client, doob_engine_process, this);
		
		jack_activate(jack_client);
		
		DBG("client activated...")
	}
	
	~engine() {
		DBG("~engine()...")
		jack_deactivate(jack_client);
		for (processors_t::iterator it = processors_begin(); it != processors_end(); ++it) {
			for (unsigned int index = 0; index < (*it)->ports.size(); ++index) {
				
			}
		}
		
		jack_client_close(jack_client);
		// commands = ringbuffer<command_t>(10);
		DBG("...~engine()")
	}
	
	processors_t::iterator processors_begin() {
		return processors->begin();
	}

	processors_t::iterator processors_end() {
		return processors->end();
	}

	processors_t::iterator processors_remove(processors_t::iterator it) {
		return processors->erase(it);
	}
	
	int process(jack_nframes_t nframes) {
		if (commands.can_read()) {
				DBG("cmd")
				commands.read()();
		}
		//std::cout << ".";
		for (
			processors_t::iterator it = processors_begin();
			it != processors_end();
			++it
		) {
			(*it)->process(nframes);
		}
		return 0;
	}
};


} // namespace


#endif
