#ifndef DICEDISPLAY_H
#define DICEDISPLAY_H

#include <QWidget>

class Dice;

class DiceDisplay : public QWidget
{
	Q_OBJECT

	Dice *dice;

public:
	DiceDisplay(Dice *setDice, QWidget *parent);

protected:
	void paintEvent(QPaintEvent *event);

};

#endif // DICEDISPLAY_H
