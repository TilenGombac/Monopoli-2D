#include "dicedisplay.h"

#include <QPainter>

#include "core/game/entity/dice.h"

DiceDisplay::DiceDisplay(Dice *setDice, QWidget *parent)
	: QWidget(parent)
{
	dice = setDice;

	connect(dice, SIGNAL(rolled()), this, SLOT(update()));

	this->setGeometry(200, 200, 300, 300);
}

void DiceDisplay::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);

	QPainter painter(this);

	QString basePath = ":/img/img/dice/die";

	QString path1 = basePath + QString::number(dice->getFirst()) + ".png";
	QString path2 = basePath + QString::number(dice->getSecond()) + ".png";

	QPixmap pixmap1(path1);
	QPixmap pixmap2(path2);

	painter.drawPixmap(0, 0, 60, 60, pixmap1);

	painter.drawPixmap(70, 70, 60, 60, pixmap2);
}
