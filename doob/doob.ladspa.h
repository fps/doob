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

struct ladspa : jack_client {
	Q_OBJECT


public:
	const LADSPA_Descriptor *descriptor;
	LADSPA_Handle instance;
	
	vector<LADSPA_Data> defaults;

	
	ladspa(string library, string label, QObject *parent = 0) :
		jack_client("doob.ladspa-" + label, parent)
	{
		void *dl = dlopen(library.c_str(), RTLD_NOW);
		
		LADSPA_Descriptor_Function ladspa_descriptor_fun;
		ladspa_descriptor_fun = (LADSPA_Descriptor_Function)dlsym(dl, "ladspa_descriptor");
		
		int index = 0;
		while(true) {
			descriptor = ladspa_descriptor_fun(index);
			if (0 == descriptor) {
				throw runtime_error("oops");
			}
			
			if (label == descriptor->Label) {
				break;
			}
			++index;
		}
		cout << descriptor->Label << endl;
		
		vector<string> input_port_names;
		vector<string> output_port_names;

		for (unsigned int index = 0; index < descriptor->PortCount; ++index) {
			if (LADSPA_IS_PORT_INPUT(descriptor->PortDescriptors[index])) {
				input_port_names.push_back(descriptor->PortNames[index]);
				cout << "default: " << descriptor->PortNames[index] << " " << get_port_default(index) << endl;
				defaults.push_back(get_port_default(index));
			} else {
				output_port_names.push_back(descriptor->PortNames[index]);
				defaults.push_back(0);
			}
		}
		
		set_port_names(vector<string>(), vector<string>(), input_port_names, output_port_names);
		
 		instance = descriptor->instantiate(descriptor, jack_get_sample_rate(client));
		if(0 != descriptor->activate) {
			descriptor->activate(instance);
		}

		activate();
	}

	~ladspa() {
		deactivate();
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
		jack_client::publishPortInformation();
		
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
		
		emit PluginNameChanged(descriptor->Name);
		emit PortsChanged(port_list);
	}

signals:
	void PluginNameChanged(const QString &Name);
	void PortsChanged(LadspaPortList Ports);
};

} // namespace

#endif