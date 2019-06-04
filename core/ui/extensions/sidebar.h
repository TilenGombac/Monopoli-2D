#ifndef SIDEBAR_H
#define SIDEBAR_H

#include "core/elements/draggablewidget.h"

class Board;
class ControlButtons;

class Sidebar : public DraggableWidget
{
	Q_OBJECT

	// Components
	ControlButtons *controlButtons;

public:
	Sidebar(QWidget *parent = 0);
	~Sidebar();

protected:
	void paintEvent(QPaintEvent *event);

private:
	void initialize();

};

#endif // SIDEBAR_H
