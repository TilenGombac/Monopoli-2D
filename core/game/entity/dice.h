#ifndef DICE_H
#define DICE_H

#include <QObject>

class Dice : public QObject
{
	Q_OBJECT

	// 1-6 each
	int value1;
	int value2;

public:
	Dice();
	int getFirst();
	int getSecond();

	void setValue(const int &first, const int &second);

public slots:
	void roll();

signals:
	void rolled();
};

#endif // DICE_H
