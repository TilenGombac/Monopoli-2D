#ifndef DRAGGABLEWIDGET_H
#define DRAGGABLEWIDGET_H

#include <QWidget>

class DraggableWidget : public QWidget
{
	Q_OBJECT

	QPoint lastMousePosition;
	bool movingWidgetFlag;

public:
	DraggableWidget(QWidget *parent = 0, Qt::WindowFlags f = Qt::WindowFlags());

protected:
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);

signals:
	void moved();
};

#endif // DRAGGABLEWIDGET_H
