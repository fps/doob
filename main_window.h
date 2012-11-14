#ifndef DOOB_MAIN_WINDOW_HH
#define DOOB_MAIN_WINDOW_HH

#include <QMainWindow>

#include <cmath>

#include <dbg.h>
#include <song_view.h>
#include <time.h>


class main_window : public QMainWindow {
	Q_OBJECT

	song_view view;

	public slots:
	void playback_position_change(doob::time time) {
		emit playback_position_changed(time);
	}
	
	signals:
	void playback_position_changed(doob::time time);

	public:
	main_window(shared_ptr<engine> the_engine) :
		view(the_engine) 
	{
		showMaximized();
		setCentralWidget(&view);
	}
};

#endif

