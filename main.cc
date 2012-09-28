#include <main_window.h>
#include <QApplication>
#include <dbg.h>

#include <singleton.h>

// #include <carla_backend/carla_backend_standalone.h>

int main(int argc, char *argv[]) {
	singleton::get();
	
	QApplication app(argc, argv);
	main_window main_window;
	main_window.show();
	
	int ret = app.exec();

	return ret;
}

