#include <main_window.h>
#include <QApplication>
#include <dbg.h>

#include <carla-backend/carla_backend_standalone.h>
#include <carla-backend/carla_plugin.h>

int main(int argc, char *argv[]) {
	for (int i = 0; i < get_engine_driver_count(); ++i) {
		DBG(get_engine_driver_name(i))
	}

	engine_init(get_engine_driver_name(0), "doob");

	QApplication app(argc, argv);
	main_window main_window;
	main_window.show();
	
	int ret = app.exec();
	engine_close();
	return ret;
}

