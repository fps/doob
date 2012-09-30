#ifndef DOOB_ENGINE_HH
#define DOOB_ENGINE_HH

#include <dbg.h>

#include <jack/jack.h>
#include <jack/midiport.h>

#include <vector>
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
using std::string;
using std::runtime_error;
using std::stringstream;

/**
 * The engine consumes midi_events and audio from tracks
 * (ultimately from patterns) and plays them back via
 * jack.
 */
struct engine {
	jack_client_t *jack_client;
	
	vector<jack_port_t*> audio_input_ports;
	vector<jack_port_t*> audio_output_ports;

	vector<jack_port_t*> midi_input_ports;
	vector<jack_port_t*> midi_output_ports;
	
	/**
	 * NOTE: this constructor might throw.
	 * Among others it might throw std::runtime_errors
	 * which give a textual representation of the
	 * reason for the failure..
	 */
	engine(
		int num_audio_input_ports = 8,
		int num_audio_output_ports = 8,
		int num_midi_input_ports = 4,
		int num_midi_output_ports = 4
      	) {
		DBG("engine()")
		
		jack_client = jack_client_open("doob", JackNullOption, NULL, 0);
		
		if (NULL == jack_client) {
			DBG("Something went wrong")
			throw runtime_error("Could not register jack client");
		}
		
		adjust_port_count(
			num_audio_input_ports,
			num_audio_output_ports,
			num_midi_input_ports,
			num_midi_output_ports
		);
		
		jack_set_process_callback(jack_client, doob_engine_process, this);
		
		jack_activate(jack_client);
		
		DBG("client activated")
	}
	
	~engine() {
		remove_all_ports();
		jack_deactivate(jack_client);
		jack_client_close(jack_client);
	}
	
	void remove_all_ports() {
		for (
			unsigned int index = 0; 
			index < audio_input_ports.size(); 
			++index) {
			
			jack_port_unregister(jack_client, audio_input_ports[index]);
		}

		for (
			unsigned int index = 0; 
			index < audio_output_ports.size(); 
			++index) {
			
			jack_port_unregister(jack_client, audio_output_ports[index]);
		}

		for (
			unsigned int index = 0; 
			index < midi_input_ports.size(); 
			++index) {
			
			jack_port_unregister(jack_client, midi_input_ports[index]);
		}

		for (
			unsigned int index = 0; 
			index < midi_output_ports.size(); 
			++index) {
			
			jack_port_unregister(jack_client, midi_output_ports[index]);
		}

	}
	
	void adjust_port_count(
		int num_audio_input_ports,
		int num_audio_output_ports,
		int num_midi_input_ports,
		int num_midi_output_ports
	) {
		remove_all_ports();
		
		for (
			unsigned int index = 0; 
			index < num_audio_input_ports; 
			++index) {
			
			stringstream stream;
			stream << "audio_in_" << index;
			
			jack_port_t *port = jack_port_register(
				jack_client,
				stream.str().c_str(),
				JACK_DEFAULT_AUDIO_TYPE,
				JackPortIsInput,
				0
			);
		
			if (NULL == port) {
				throw runtime_error("could not register audio port " + stream.str());
			}
			
			audio_input_ports.push_back(port);
		}

		for (
			unsigned int index = 0; 
			index < num_audio_output_ports; 
			++index) {
			
			stringstream stream;
			stream << "audio_out_" << index;
			
			jack_port_t *port = jack_port_register(
				jack_client,
				stream.str().c_str(),
				JACK_DEFAULT_AUDIO_TYPE,
				JackPortIsOutput,
				0
			);
		
			if (NULL == port) {
				throw runtime_error("could not register audio port " + stream.str());
			}
			
			audio_output_ports.push_back(port);
		}

		for (
			unsigned int index = 0; 
			index < num_midi_input_ports; 
			++index) {
			
			stringstream stream;
			stream << "midi_in_" << index;
			
			jack_port_t *port = jack_port_register(
				jack_client,
				stream.str().c_str(),
				JACK_DEFAULT_MIDI_TYPE,
				JackPortIsInput,
				0
			);
		
			if (NULL == port) {
				throw runtime_error("could not register midi port " + stream.str());
			}

			midi_input_ports.push_back(port);
		}

		for (
			unsigned int index = 0; 
			index < num_midi_output_ports; 
			++index) {
			
			stringstream stream;
			stream << "midi_out_" << index;
			
			jack_port_t *port = jack_port_register(
				jack_client,
				stream.str().c_str(),
				JACK_DEFAULT_MIDI_TYPE,
				JackPortIsOutput,
				0
			);
			
			if (NULL == port) {
				throw runtime_error("could not register midi port " + stream.str());
			}
		
			midi_output_ports.push_back(port);
		}
	}

	int process(jack_nframes_t) {
		//std::cout << ".";
		return 0;
	}
};


};




#endif
