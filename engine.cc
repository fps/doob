#include <engine.h>

extern "C" { 
	int doob_engine_process(jack_nframes_t nframes, void *arg) {
		((doob::engine*)arg)->process(nframes);
	}
}