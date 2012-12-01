#include <doob/doob.ladspa.h>
#include <doob/ladspa_adaptor.h>
#include <doob/jack_client_adaptor.h>
#include <doob/doob.meta_types.h>

#include <boost/program_options.hpp>

#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>

#include <signal.h>

#include <QtCore/QCoreApplication>
#include <QtDBus/QDBusConnection>
#include <QDBusArgument>

using std::cout;
using std::endl;

void signal_handler(int signum) {
	cout << "\nSignalled: " << signum << " - Shutting down..." << endl;
	QCoreApplication::quit();
}

int main (int argc, char *argv[]) {
	register_meta_types();
	
	// cout << "############################" << endl;
	using std::string;
	using std::runtime_error;

	signal(2, signal_handler);
	
	using namespace boost::program_options;
	options_description desc;
	desc.add_options()
		("help,h", "produce help output")
		("library,l", value<string>()->required(), "the ladspa library file")
		("label,n", value<string>()->required(), "the ladspa plugin lable")
		("service-name,s", value<string>(), "the dbus service name to register")
	;

	variables_map vm;
	store(parse_command_line(argc, argv, desc), vm);
	
	if(vm.count("help")) {
		cout << desc << endl;
		return EXIT_SUCCESS;
	}

	try {
		notify(vm);
	} catch(error e) {
		cout << e.what() << endl;
		return EXIT_FAILURE;
	}

	QCoreApplication app(argc, argv);
	try {
		cout << "service-name: " << vm["service-name"].as<string>() << endl;

		doob::ladspa l(vm["library"].as<string>(), vm["label"].as<string>(), 0);	
		
		
		new LadspaAdaptor(&l);
		new Jack_clientAdaptor(&l);
		
		//a->connect(&l, SIGNAL(PortsChanged(LadspaPortList)), a, SIGNAL(PortsChanged(LadspaPortList)));
		
		if (false == QDBusConnection::sessionBus().registerService(vm["service-name"].as<string>().c_str())) {
			throw runtime_error("failed to register service name");
		}


		if (false == QDBusConnection::sessionBus().registerObject("/Plugin", &l)) {
			throw runtime_error("failed to register object name");
		}
		
		l.publishPortInformation();
		
		return app.exec();

	} catch (runtime_error e) {
		cout << e.what() << endl;
		return EXIT_FAILURE;
	}
}