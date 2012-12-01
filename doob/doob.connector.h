#ifndef POOQ_LADSPA_HH
#define POOQ_LADSPA_HH

#include <ladspa.h>

#include <doob/doob.meta_types.h>
#include <doob/doob.jack_client.h>

#include <dlfcn.h>

#include <algorithm>
#include <stdexcept>

#include <QObject>
#include <QCoreApplication>
#include <QList>
#include <QPair>

namespace doob {

using std::string;
using std::vector;
using std::fill;
using std::runtime_error;

struct connector : jack_client {
	Q_OBJECT


public:
	const LADSPA_Descriptor *descriptor;
	LADSPA_Handle instance;
	
	vector<LADSPA_Data> defaults;

	
	connector(string source, string sink, QObject *parent = 0) :
		jack_client("doob-connector", parent)
	{
		jack_activate(client);
	}

	~connector() {
		jack_deactivate(client);
	}
	
	LADSPA_Data get_port_default(unsigned int port_index) {
		LADSPA_PortRangeHintDescriptor x = descriptor->PortRangeHints[port_index].HintDescriptor;
		float def = 0.0f;
		if (LADSPA_IS_HINT_BOUNDED_BELOW(x)) { }
		if (LADSPA_IS_HINT_BOUNDED_ABOVE(x)) { }
		if (LADSPA_IS_HINT_TOGGLED(x)) { }
		if (LADSPA_IS_HINT_LOGARITHMIC(x)) { }
		if (LADSPA_IS_HINT_INTEGER(x)) { } 

		if (LADSPA_IS_HINT_DEFAULT_440(x)) {
			return 440.0f;
		}
		if (LADSPA_IS_HINT_DEFAULT_100(x)) {
			return 100.0f;
		}
		if (LADSPA_IS_HINT_DEFAULT_1(x)) {
			return  1.0f;
		}
		if (LADSPA_IS_HINT_DEFAULT_0(x)) {
			return  0.0f;
		}
		
		if (LADSPA_IS_HINT_SAMPLE_RATE(x)) { 
			if (LADSPA_IS_HINT_DEFAULT_MINIMUM(x)) {
				def =  descriptor->PortRangeHints[port_index].LowerBound;
			}
											
			if (LADSPA_IS_HINT_DEFAULT_LOW(x)) { 
				def =  (descriptor->PortRangeHints[port_index].UpperBound + descriptor->PortRangeHints[port_index].LowerBound) / 2;
			}   
											
			if (LADSPA_IS_HINT_DEFAULT_MIDDLE(x)) { 
				def = (descriptor->PortRangeHints[port_index].UpperBound + descriptor->PortRangeHints[port_index].LowerBound) / 2;
			}
												
			if (LADSPA_IS_HINT_DEFAULT_HIGH(x)) {
				def =  (descriptor->PortRangeHints[port_index].UpperBound + descriptor->PortRangeHints[port_index].LowerBound) / 2;
			}   
											
			if (LADSPA_IS_HINT_DEFAULT_MAXIMUM(x)) { 
				def =  descriptor->PortRangeHints[port_index].UpperBound;
			}
			return jack_get_sample_rate(client) * def; 
		} else {
			if (LADSPA_IS_HINT_DEFAULT_MINIMUM(x)) {
				def =  descriptor->PortRangeHints[port_index].LowerBound;
			}
											
			if (LADSPA_IS_HINT_DEFAULT_LOW(x)) { 
				def =  (descriptor->PortRangeHints[port_index].UpperBound + descriptor->PortRangeHints[port_index].LowerBound) / 2;
			}   
											
			if (LADSPA_IS_HINT_DEFAULT_MIDDLE(x)) { 
				def = (descriptor->PortRangeHints[port_index].UpperBound + descriptor->PortRangeHints[port_index].LowerBound) / 2;
			}
												
			if (LADSPA_IS_HINT_DEFAULT_HIGH(x)) {
				def =  (descriptor->PortRangeHints[port_index].UpperBound + descriptor->PortRangeHints[port_index].LowerBound) / 2;
			}   
											
			if (LADSPA_IS_HINT_DEFAULT_MAXIMUM(x)) { 
				def =  descriptor->PortRangeHints[port_index].UpperBound;
			}
			
		}
		return def;
	}

	virtual int process(jack_nframes_t nframes) {
		for (jack_nframes_t frame = 0; frame < nframes; ++frame) {
			unsigned int audio_in = 0;
			unsigned int audio_out = 0;
		
			for (unsigned int index = 0; index < descriptor->PortCount; ++index) {
				if (LADSPA_IS_PORT_INPUT(descriptor->PortDescriptors[index])) {
					if (jack_port_connected(audio_in_ports[audio_in])) {
						descriptor->connect_port(instance, index, (LADSPA_Data*)jack_port_get_buffer(audio_in_ports[audio_in++], nframes) + frame);
					} else {
						descriptor->connect_port(instance, index, &defaults[index]);
					}
				} else {
					descriptor->connect_port(instance, index, (LADSPA_Data*)jack_port_get_buffer(audio_out_ports[audio_out++], nframes) + frame);
				}
			}
			descriptor->run(instance, 1);
		}
		
		return 0;
	}

public slots:
	void Quit() {
		jack_client::Quit();
	}
	
void publishPortInformation() {
		unsigned int audio_in = 0;
		unsigned int audio_out = 0;

		QList<LadspaPort> port_list;
		for (unsigned int index = 0; index < descriptor->PortCount; ++index) {
			QString jack_name = "";
			if (LADSPA_IS_PORT_INPUT(descriptor->PortDescriptors[index])) {
				jack_name = jack_port_name(audio_in_ports[audio_in++]);
			} else {
				jack_name = jack_port_name(audio_out_ports[audio_out++]);
			}
			port_list << LadspaPort(
				descriptor->PortNames[index], 
				(int)descriptor->PortDescriptors[index], 
				(int)descriptor->PortRangeHints[index].HintDescriptor, 
				jack_name
			);
		}
		
		emit PortsChanged(port_list);
		emit Ready(jack_get_client_name(client));
	}

signals:
	void Ready(QString JackName);
	
	void PortsChanged(LadspaPortList Ports);
	
#if 0
	virtual void get_name(std::string& name) { name = this->name; }
	
	virtual void set_name(const string&) { }
	
	virtual void set_port(int32_t, double) { }
		
	virtual void load_plugin(const std::string& library, const std::string& label, const int32_t polyphony) { }
	
	virtual void set_enabled(const int32_t ok) { }
	
	virtual int32_t alive() { return 1; }
	
	virtual void set_activated(const int32_t ok) { /** TODO: handle correctly */ }

	virtual int32_t get_activated() { return 1; /** TODO: handle correctly */ }
	
	virtual void get_ports(std::vector<port>& ports) { 
		for (unsigned long index = 0; index < descriptor->PortCount; ++index) {
			port p;
			p.name = descriptor->PortNames[index];

			if(
				LADSPA_IS_PORT_INPUT(descriptor->PortDescriptors[index])
				&& LADSPA_IS_PORT_CONTROL(descriptor->PortDescriptors[index])
			) {
				p.type = port_type::control;
				p.direction = port_direction::sink;
				p.description = ("Control Port | Sink");
			}
			if(
				LADSPA_IS_PORT_OUTPUT(descriptor->PortDescriptors[index])
				&& LADSPA_IS_PORT_CONTROL(descriptor->PortDescriptors[index])
			) {
				p.type = (port_type::control);
				p.direction = (port_direction::source);
				p.description = ("Control Port | Source");
			}
			if(
				LADSPA_IS_PORT_INPUT(descriptor->PortDescriptors[index])
				&& LADSPA_IS_PORT_AUDIO(descriptor->PortDescriptors[index])
			) {
				p.type = (port_type::audio);
				p.direction = (port_direction::sink);
				p.description = ("Audio Port | Sink");
			}
			if(
				LADSPA_IS_PORT_OUTPUT(descriptor->PortDescriptors[index])
				&& LADSPA_IS_PORT_AUDIO(descriptor->PortDescriptors[index])
			) {
				p.type = (port_type::audio);
				p.direction = (port_direction::source);
				p.description = ("Audio Port | Source");
			}
			ports.push_back(p);
		}
	}
#endif
};

} // namespace

#endif