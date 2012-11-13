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

	shared_ptr<engine> e(new engine());

	midi_track_ptr mp(
		new midi_track(e, "midi1", 1)
	);

	engine::processors_t_ptr processors(
		new engine::processors_t(*(e->processors))
	);
	
	processors->push_back(mp);

	e->commands.write(
		assign(e->processors, processors)
	);

	{
		QApplication app(argc, argv);
		main_window main_window;
		main_window.show();
		ret = app.exec();
	}


	DBG("exiting...")
	return ret;
}

