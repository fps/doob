#include <main_window.h>
#include <QApplication>
#include <dbg.h>

#include <singleton.h>
#include <midi_processor.h>
#include <assign.h>

#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
// #include <carla_backend/carla_backend_standalone.h>

using boost::shared_ptr;

using namespace doob;

int main(int argc, char *argv[]) {
	singleton_ptr s = singleton::get();
	int ret = 0;
	
	{
		midi_processor_ptr mp(new midi_processor(s->the_engine, "midi1", 1));
		
		engine::processors_t_ptr processors(new engine::processors_t(*(s->the_engine->processors)));
		processors->push_back(mp);
		s->the_engine->commands.write(assign(s->the_engine->processors, processors));

		QApplication app(argc, argv);
		main_window main_window;
		main_window.show();

		ret = app.exec();
	}
	
	s->remove();
	
	return ret;
}

