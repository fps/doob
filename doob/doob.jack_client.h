#ifndef SEQPP_JACK_CLIENT_HH
#define SEQPP_JACK_CLIENT_HH

#include <jack/jack.h>
#include <jack/midiport.h>
#include <string>
#include <stdexcept>
#include <iostream>
#include <vector>

#include <doob/jack_client_adaptor.h>

#include <QObject>
#include <QCoreApplication>

namespace doob {


using std::cout;
using std::endl;
using std::string;
using std::runtime_error;
using std::vector;

extern "C" int jack_client_process(jack_nframes_t nframes, void *arg);

struct jack_client : QObject {
	Q_OBJECT

public:
	string name;
	
	jack_client_t *client;

	vector<jack_port_t*> midi_in_ports;
	vector<jack_port_t*> midi_out_ports;

	vector<jack_port_t*> audio_in_ports;
	vector<jack_port_t*> audio_out_ports;

	jack_client(
		const string name,
		QObject *parent = 0
	) throw (runtime_error) :
		QObject(parent),
		name(name)
	{
		cout << "jack_client() name size: " << jack_client_name_size() << endl;
		jack_status_t jack_status;

		client = jack_client_open(name.substr(0, jack_client_name_size() - 1).c_str(), JackNullOption, &jack_status);

		if (0 == client) {
			throw runtime_error("failed to open jack client");
		}

		jack_set_process_callback(client, doob::jack_client_process, (void*)this);

		// jack_activate(client);
	}

	void activate() {
		jack_activate(client);
	}
	
	void deactivate() {
		jack_deactivate(client);
	}

	void set_port_names(
		vector<string> midi_in_port_names,
		vector<string> midi_out_port_names,
		vector<string> audio_in_port_names,
		vector<string> audio_out_port_names
	) {
		for (unsigned int index = 0; index < midi_in_port_names.size(); ++index) {
			cout << "registering port: " << midi_in_port_names[index] << std::endl;
			jack_port_t *port = jack_port_register(client, midi_in_port_names[index].c_str(), JACK_DEFAULT_MIDI_TYPE, JackPortIsTerminal | JackPortIsInput, 0);
			midi_in_ports.push_back(port);
		}

		for (unsigned int index = 0; index < midi_out_port_names.size(); ++index) {
			cout << "registering port: " << midi_out_port_names[index] << std::endl;
			jack_port_t *port = jack_port_register(client, midi_out_port_names[index].c_str(), JACK_DEFAULT_MIDI_TYPE, JackPortIsTerminal | JackPortIsOutput, 0);
			midi_out_ports.push_back(port);
		}

		for (unsigned int index = 0; index < audio_in_port_names.size(); ++index) {
			cout << "registering port: " << audio_in_port_names[index] << std::endl;
			jack_port_t *port = jack_port_register(client, audio_in_port_names[index].c_str(), JACK_DEFAULT_AUDIO_TYPE, JackPortIsTerminal | JackPortIsInput, 0);
			audio_in_ports.push_back(port);
		}

		for (unsigned int index = 0; index < audio_out_port_names.size(); ++index) {
			cout << "registering port: " << audio_out_port_names[index] << std::endl;
			jack_port_t *port = jack_port_register(client, audio_out_port_names[index].c_str(), JACK_DEFAULT_AUDIO_TYPE, JackPortIsTerminal | JackPortIsOutput, 0);
			audio_out_ports.push_back(port);
		}

	}
	
	virtual ~jack_client() {
		cout << "~jack_client()" << endl;

		// jack_deactivate(client);

		for (unsigned int index = 0; index < midi_in_ports.size(); ++index) {
			jack_port_unregister(client, midi_in_ports[index]);
		}

		for (unsigned int index = 0; index < midi_out_ports.size(); ++index) {
			jack_port_unregister(client, midi_out_ports[index]);
		}

		for (unsigned int index = 0; index < audio_in_ports.size(); ++index) {
			jack_port_unregister(client, audio_in_ports[index]);
		}

		for (unsigned int index = 0; index < audio_out_ports.size(); ++index) {
			jack_port_unregister(client, audio_out_ports[index]);
		}

		jack_client_close(client);
	}

	int do_process(jack_nframes_t nframes) {
		for (unsigned int index = 0; index < midi_out_ports.size(); ++index) {
			jack_midi_clear_buffer(jack_port_get_buffer(midi_out_ports[index], nframes));
		}

		return process(nframes);
	}

	/*
		override this method. note that for all midi output ports
		jack_midi_clear_buffer has been called
	*/
	virtual int process(jack_nframes_t nframes) = 0;

	virtual void publishPortInformation() {
		emit JackNameChanged(jack_get_client_name(client));
		
		QList<JackPort> port_list;

		for (unsigned int index = 0, max = midi_in_ports.size(); index < max; ++index) {
			port_list << JackPort(jack_port_name(midi_in_ports[index]), 1, 0, 0, 1);
		}
		for (unsigned int index = 0, max = midi_out_ports.size(); index < max; ++index) {
			port_list << JackPort(jack_port_name(midi_out_ports[index]), 0, 1, 0, 1);
		}
		for (unsigned int index = 0, max = audio_in_ports.size(); index < max; ++index) {
			port_list << JackPort(jack_port_name(audio_in_ports[index]), 1, 0, 1, 0);
		}
		for (unsigned int index = 0, max = audio_out_ports.size(); index < max; ++index) {
			port_list << JackPort(jack_port_name(audio_out_ports[index]), 0, 1, 1, 0);
		}
		
		emit PortsChanged(port_list);
	}
signals:
	void JackNameChanged(const QString &JackName);
	void PortsChanged(JackPortList Ports);

public slots:
	void Quit() {
		QCoreApplication::exit();
	}

};

} // namespace


#endif

