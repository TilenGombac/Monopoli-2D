#include "draggablewidget.h"

#include <QMouseEvent>

DraggableWidget::DraggableWidget(QWidget *parent, Qt::WindowFlags f)
	: QWidget(parent, f)
{
	movingWidgetFlag = false;
}

void DraggableWidget::mouseMoveEvent(QMouseEvent *event)
{
	if(movingWidgetFlag)
	{
		QPoint positionChange = event->pos() - lastMousePosition;

		QWidget *parent = this->parentWidget();

		// Draggable widget has no parent
		if(parent == 0)
		{
			parent = this;
		}

		parent->move(parent->pos() + positionChange);

		emit moved();
	}
}

void DraggableWidget::mousePressEvent(QMouseEvent *event)
{
	if(event->button() == Qt::LeftButton)
	{
		movingWidgetFlag = true;

		lastMousePosition = event->pos();
	}
}

void DraggableWidget::mouseReleaseEvent(QMouseEvent *event)
{
	Q_UNUSED(event);

	movingWidgetFlag = false;
}
