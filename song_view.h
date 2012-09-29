#ifndef DOOB_SONG_VIEW_HH
#define DOOB_SONG_VIEW_HH

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include <QGraphicsWidget>
#include <QGraphicsLineItem>
#include <QTextEdit>
#include <QBrush>
#include <QColor>
#include <QRectF>
#include <QFont>
#include <QVBoxLayout>
#include <QScrollBar>

#include <types.h>
#include <dbg.h>

#include <cmath>

using namespace doob;

class sequenced_item : public QGraphicsWidget {
	Q_OBJECT

	typedef int lane_size_t;
	
	/*
		There's MAX_INT potential lanes
	*/
	lane_size_t lane;

	/*
		the tick that this item is positioned at
	*/
	tick_t tick;

	public:
	lane_size_t get_lane() {
		return lane;
	}

	void set_lane(lane_size_t l) {
		lane = l;
	}

	sequenced_item(lane_size_t lane = 0) : lane(lane) {

	}
};

class text_item : public sequenced_item {
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
	static int line_height() {
		return 17;
	}

	text_item() { 
		rect = new QGraphicsRectItem();
		edit = new QGraphicsTextItem();

		edit->setTextInteractionFlags(Qt::TextEditorInteraction);
		edit->setPlainText("c eb g\n\nc eb g\n\nc eb g\n\nc eb g\n\nc eb g\n\nc eb g\n\nc eb g\n\nc eb g\n");
		edit->setDefaultTextColor(QColor(255, 244, 233));
		edit->setY(edit->y() - 2);
		{
			QFont font = edit->document()->defaultFont();
			font.setFamily("monospace");
			font.setFixedPitch(true);
			font.setPixelSize(14);
			edit->document()->setDefaultFont(font);
		}
		edit->setParentItem(rect);

		QPen pen(QColor(255, 255, 255, 64));
		pen.setWidth(4);
		rect->setPen(pen);
		rect->setY(rect->y() - 2);
		rect->setRect(QRectF(-100, -100, 200, 200));
		rect->setBrush(QColor(255, 255, 255, 32));
		rect->setParentItem(this);

		connect(edit->document(), SIGNAL(contentsChanged()), this, SLOT(changed()));
		changed();
	}
};

class range_item : public QGraphicsWidget {
	Q_OBJECT

	/*
		the range is defined as those ticks >= tick_start and < tick_end
	*/
	tick_t start;
	tick_t end;

	QGraphicsRectItem *rect;

	QGraphicsLineItem *range_line;
	QGraphicsLineItem *start_line;
	QGraphicsLineItem *end_line;

	public slots:
	void view_changed() {
		DBG("view changed")
		QGraphicsView *view = scene()->views().at(0);

		// setX(scene()->views().at(0)->mapToScene(10, 0).x());
		QRectF r = rect->rect();

		int width = view->viewport()->width();

		r.setX(view->mapToScene(0, 0).x());
		r.setWidth(view->mapToScene(width, 0).x() - view->mapToScene(0, 0).x());
			
		rect->setRect(r);
	}

	public:
	range_item() {
		rect = new QGraphicsRectItem();
		rect->setParentItem(this);

		QPen pen(QColor(255, 55, 5));
		pen.setWidth(8);
		rect->setPen(pen);

		QBrush brush(QColor(255, 33, 54, 16));
		rect->setBrush(brush);

		set_range(0, 16);

		setOpacity(0.5);
	}

	 void set_range(tick_t start_tick, tick_t end_tick) {
		start = start_tick;
		end = end_tick;

		rect->setRect(QRectF(0, start * text_item::line_height(), 0, (end - start) * text_item::line_height()));
	}
};

class song_view : public QGraphicsView {
	Q_OBJECT

	QGraphicsScene scene;

	/*
		< 0 -> zoom out 
		  0 -> zoom factor 1
		> 0 -> zoom in
	*/
	typedef int scroll_level_size_t;
	scroll_level_size_t scroll_level;

	public:
	song_view() : scroll_level(0) {
		setScene(&scene);
		scene.setBackgroundBrush(QColor(69/4, 130/4, 90/4));

		range_item *range = new range_item();
		scene.addItem(range);
		connect(this, SIGNAL(view_changed()), range, SLOT(view_changed()));

		connect(horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(view_change()));	
		connect(verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(view_change()));	
		emit view_changed();
	}

	signals:
	void view_changed();

	protected slots:
	void view_change() { 
		emit view_changed();
	}

	public slots:
	void playback_position_change(tick_t tick) {
		
	}

	protected:
	virtual void wheelEvent (QWheelEvent *event) {
		DBG("wheel delta: " << event->delta());
		if (event->modifiers() & Qt::ControlModifier) {
			if (event->delta() < 0) {
				--scroll_level;
			}

			if (event->delta() > 0) {
				++scroll_level;
			}

			QTransform the_transform = transform();
			QTransform new_transform;
			new_transform.translate(the_transform.dx(), the_transform.dy());
			double scale = pow(1.05, scroll_level);
			new_transform.scale(scale, scale);
			setTransform(new_transform);
			event->accept();
		} 
		else {
			QGraphicsView::wheelEvent(event);
		}

		emit view_changed();
	}

	void resizeEvent (QResizeEvent *event) {
		QGraphicsView::resizeEvent(event);
		emit view_changed();
	}
};


#endif
