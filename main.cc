#include <main_window.h>
#include <QApplication>
#include <dbg.h>

#include <singleton.h>
#include <midi_track.h>
#include <assign.h>

#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
// #include <carla_backend/carla_backend_standalone.h>

using boost::shared_ptr;

using namespace doob;

int main(int argc, char *argv[]) {
	int ret = 0;

	shared_ptr<engine> the_engine(new engine());

	midi_track_ptr mp(
		new midi_track(the_engine, "midi1", 1)
	);

	engine::processors_t_ptr processors(
		new engine::processors_t(*(the_engine->processors))
	);
	
	processors->push_back(mp);

	the_engine->commands.write(
		assign(the_engine->processors, processors)
	);

	{
		QApplication app(argc, argv);
		main_window main_window(the_engine);
		main_window.show();
		ret = app.exec();
	}


	DBG("exiting...")
	return ret;
}

