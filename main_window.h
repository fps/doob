#ifndef DOOB_MAIN_WINDOW_HH
#define DOOB_MAIN_WINDOW_HH

#include <QMainWindow>

#include <cmath>

#include <dbg.h>
#include <song_view.h>


class main_window : public QMainWindow {
	Q_OBJECT

	song_view view;

	public slots:
	void playback_position_change(tick_size_t tick) {
		emit playback_position_changed(tick);
	}
	
	signals:
	void playback_position_changed(tick_size_t tick);

	public:
	main_window() {
		showMaximized();

		setCentralWidget(&view);
	}
};

#endif

