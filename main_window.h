#ifndef DOOB_MAIN_WINDOW_HH
#define DOOB_MAIN_WINDOW_HH

#include <QMainWindow>
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

#include <cmath>

#include <dbg.h>

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

typedef long long tick_size_t;

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
	tick_size_t tick;

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
	tick_size_t start;
	tick_size_t end;

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

	/*
	protected:
	virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value) {
		DBG("item change")
		if (change == ItemTransformChange && scene()) { 
			QPointF new_pos = value.toPointF();	
			QRectF rect = scene()->sceneRect();
			new_pos.setX(rect.x() + 10);
			return new_pos;
		}
		return QGraphicsItem::itemChange(change, value);
	}
	*/

	public:
	range_item() {
		rect = new QGraphicsRectItem();
		rect->setParentItem(this);

		QPen pen(QColor(255, 55, 5));
		pen.setWidth(8);
		rect->setPen(pen);

		QBrush brush(QColor(255, 33, 54, 16));
		rect->setBrush(brush);

#if 0
		range_line = new QGraphicsLineItem();
		start_line = new QGraphicsLineItem();
		end_line = new QGraphicsLineItem();
	
		range_line->setParentItem(this);
		start_line->setParentItem(this);
		end_line->setParentItem(this);
#endif
		set_range(0, 16);

		setOpacity(0.5);
	}

	 void set_range(tick_size_t start_tick, tick_size_t end_tick) {
		start = start_tick;
		end = end_tick;

#if 0
		range_line->setLine(0, start * text_item::line_height(), 0, end * text_item::line_height());
		range_line->setPen(pen);

		start_line->setLine(8, start * text_item::line_height(), 20, start * text_item::line_height());
		start_line->setPen(pen);

		end_line->setLine(8, end * text_item::line_height(), 20, end * text_item::line_height());
		end_line->setPen(pen);
#endif
		rect->setRect(QRectF(0, start * text_item::line_height(), 0, (end - start) * text_item::line_height()));
	}
};

class sequence_view : public QGraphicsView {
	Q_OBJECT

	/*
		< 0 -> zoom out 
		  0 -> zoom factor 1
		> 0 -> zoom in
	*/
	typedef int scroll_level_size_t;
	scroll_level_size_t scroll_level;

	public:
	sequence_view() : scroll_level(0) {
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
#if 0
	bool viewportEvent (QEvent * event) {
		if (event->type() != QEvent::Paint) {
			emit view_changed();
		}

		QGraphicsView::viewportEvent(event);
	}
#endif
};


class main_window : public QMainWindow {
	Q_OBJECT

	sequence_view view;
	QGraphicsScene scene;

	text_item item;

	public:
	main_window() {
		showMaximized();
		view.setScene(&scene);

		scene.setBackgroundBrush(QColor(69/4, 130/4, 90/4));
		setCentralWidget(&view);

		range_item *range = new range_item();
		scene.addItem(range);
		connect(&view, SIGNAL(view_changed()), range, SLOT(view_changed()));
#if 0		
		text_item *item1 = new text_item();
		scene.addItem(item1);

		text_item *item2 = new text_item();
		item2->setY(17 * 16);
		scene.addItem(item2);

		text_item *item3 = new text_item();
		item3->setX(item1->childrenBoundingRect().width() + 10);
		item3->setY(17 * 4);
		scene.addItem(item3);

#endif

		for (int i = 0; i < 256; ++i) {
			for (int j = 0; j < 32; ++j) {
				text_item *item2 = new text_item();
				item2->setY(text_item::line_height() * i * 16);
				item2->setX(j * (item2->childrenBoundingRect().width() + 10));
				scene.addItem(item2);
			}
		}

		for (int i = -10000; i < 10000; ++i) {
			QGraphicsRectItem *line = new QGraphicsRectItem();
			line->setRect(-10000, 2 * i * text_item::line_height(), 20000, text_item::line_height());
			line->setBrush(QColor(255, 255, 255, 16));
			line->setPen(QColor(255, 255, 255, 0));
			scene.addItem(line);
		}
	}
};

#endif

