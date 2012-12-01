#ifndef POOQ_MIDI_NOTE_HH
#define POOQ_MIDI_NOTE_HH

#include <doob/doob.jack_client.h>

#include <jack/midiport.h>

#include <dlfcn.h>

#include <algorithm>
#include <stdexcept>
#include <cmath>

namespace doob {

using std::string;
using std::vector;
using std::fill;
using std::runtime_error;

struct midi_note : virtual jack_client {
	Q_OBJECT
	
public:
	float frequency;
	float gate;
	float velocity;
	
	midi_note() :
		jack_client("doob-midi"),
		frequency(440.0),
		gate(0.0),
		velocity(0.0)
	{
		vector<string> midi_in_names;
		midi_in_names.push_back("midi_in");

		vector<string> audio_out_names;
		audio_out_names.push_back("trigger_out");
		audio_out_names.push_back("gate_out");
		audio_out_names.push_back("freq_out");
		audio_out_names.push_back("velocity_out");

		set_port_names(midi_in_names, vector<string>(), vector<string>(), audio_out_names);
		
		activate();
	}
	
	~midi_note() {
		deactivate();
	}
	
	float note_frequency(unsigned int note) {
		return (2.0 * 440.0 / 32.0) * pow(2, (((jack_default_audio_sample_t)note - 9.0) / 12.0));
	}
	
	virtual int process(jack_nframes_t nframes) {
		jack_midi_clear_buffer(jack_port_get_buffer(midi_out_ports[0], nframes));
		
		if(jack_port_connected(midi_in_ports[0])) {
			void *midi_in_buffer = jack_port_get_buffer(midi_in_ports[0], nframes);
			
			float *trigger_out_buffer = (float*)jack_port_get_buffer(audio_out_ports[0], nframes);
			float *gate_out_buffer = (float*)jack_port_get_buffer(audio_out_ports[1], nframes);
			float *freq_out_buffer = (float*)jack_port_get_buffer(audio_out_ports[2], nframes);
			float *velocity_out_buffer = (float*)jack_port_get_buffer(audio_out_ports[3], nframes);
			
			jack_midi_event_t midi_in_event;

			jack_nframes_t event_count = jack_midi_get_event_count(midi_in_buffer);
			jack_nframes_t event_index = 0;
			if (event_count > 0) {
				//cout << "ev" << endl;
				jack_midi_event_get(&midi_in_event, midi_in_buffer, event_index);
			}
			
			for (jack_nframes_t frame = 0, max = nframes; frame < max; ++frame) {
				trigger_out_buffer[frame] = 0;
				
				while (event_index < event_count && midi_in_event.time == frame) {
					//cout << "event" << endl;
					
					if( ((*(midi_in_event.buffer) & 0xf0)) == 0x90 ) 	{
						/* note on */
						frequency = note_frequency(*(midi_in_event.buffer + 1));
						velocity = *(midi_in_event.buffer + 2) / 128.0;
						//cout << "freq: " << frequency << endl;
						trigger_out_buffer[frame] = 1.0;
						gate = 1.0;
					} else if( ((*(midi_in_event.buffer)) & 0xf0) == 0x80 ) {
						/* note off */
						frequency = note_frequency(*(midi_in_event.buffer + 1));
						velocity = *(midi_in_event.buffer + 2) / 128.0;
						gate = 0.0;
					}

					freq_out_buffer[frame] = frequency;
					gate_out_buffer[frame] = gate;
					velocity_out_buffer[frame] = velocity;
					
					++event_index;
					/**
					 * TODO: Check return value for ENODATA
					 */
					jack_midi_event_get(&midi_in_event, midi_in_buffer, event_index);
				}
				
				gate_out_buffer[frame] = gate;
				freq_out_buffer[frame] = frequency;
			}
		}
		
		return 0;
	}
};

} // namespace

#endif