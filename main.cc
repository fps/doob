#include <main_window.h>
#include <QApplication>

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);
	main_window the_main_window;
	the_main_window.show();
	return app.exec();
}

