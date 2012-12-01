#include <doob/doob.jack_client.h>

namespace doob {

int jack_client_process(jack_nframes_t nframes, void *arg) {
	return ((jack_client*)arg)->do_process(nframes);	
}

} // namespace