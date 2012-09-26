#ifndef DOOB_MAIN_WINDOW_HH
#define DOOB_MAIN_WINDOW_HH

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include <QGraphicsWidget>
#include <QTextEdit>
#include <QBrush>
#include <QColor>
#include <QRectF>
#include <QFont>
#include <QVBoxLayout>


#if 0
class text_item : public QGraphicsObject {
	Q_OBJECT

	QGraphicsTextItem the_text_item;

	public:
	text_item() : the_text_item(this) {
		the_text_item.setTextInteractionFlags(Qt::TextEditorInteraction);
	}
};
#endif

class text_item : public QGraphicsWidget {
	Q_OBJECT

	QGraphicsRectItem *rect;
	QGraphicsTextItem *edit;

	public slots:
	void changed() {
		QRectF edit_rect = edit->boundingRect();
		edit_rect.setTop(edit_rect.top() + 2);
		edit_rect.setBottom(edit_rect.bottom() - 6);

		rect->setRect(edit_rect);
	}
	
	public:
	text_item() { 
		rect = new QGraphicsRectItem();
		edit = new QGraphicsTextItem();

		edit->setTextInteractionFlags(Qt::TextEditorInteraction);
		edit->setPlainText("c\nc\nc\n\n\n\nc");
		edit->setDefaultTextColor(QColor(255, 244, 233));
		edit->setY(edit->y() - 2);
		QFont font = edit->document()->defaultFont();
		font.setFamily("monospace");
		font.setFixedPitch(true);
		font.setPixelSize(14);
		edit->document()->setDefaultFont(font);
		rect->setBrush(QColor(255, 255, 255, 64));
		edit->setParentItem(rect);

		QPen pen(QColor(255, 255, 255, 128));
		pen.setWidth(4);
		rect->setPen(pen);
		rect->setY(rect->y() - 2);
		rect->setRect(QRectF(-100, -100, 200, 200));
		rect->setParentItem(this);

		connect(edit->document(), SIGNAL(contentsChanged()), this, SLOT(changed()));
		changed();
	}
};



class main_window : public QMainWindow {
	Q_OBJECT

	QGraphicsView view;
	QGraphicsScene scene;

	text_item item;

	public:
	main_window() {
		showMaximized();
		view.setScene(&scene);
		scene.setBackgroundBrush(QColor(69/2, 130/2, 90/2));
		setCentralWidget(&view);

		scene.addItem(&item);

		for (int i = -10000; i < 10000; ++i) {
			QGraphicsRectItem *line = new QGraphicsRectItem();
			line->setRect(-10000, 2 * i * 17, 20000, 17);
			line->setBrush(QColor(255, 255, 255, 16));
			line->setPen(QColor(255, 255, 255, 0));
			scene.addItem(line);
		}
	}
};

#endif

