#include "dice.h"

#include <QTime>

#include "core/utils/errorUtils/errorhandler.h"

#include "core/sounds/soundmanager.h"

Dice::Dice()
	: QObject()
{
	qsrand(QTime::currentTime().msec());

	value1 = 1;
	value2 = 1;
}

int Dice::getFirst()
{
	return value1;
}

int Dice::getSecond()
{
	return value2;
}

void Dice::setValue(const int &first, const int &second)
{
	if((first >= 1) && (first <= 6) && (second >= 1) && (second <= 6))
	{
		SoundManager::play(Sound::DICE);

		value1 = first;
		value2 = second;

		emit rolled();
	}
}

void Dice::roll()
{
	SoundManager::play(Sound::DICE);

	value1 = qrand() % 6 + 1;
	value2 = qrand() % 6 + 1;

	ErrorHandler::log("Dice roll", QString::number(value1)
						+ ", " + QString::number(value2));

	emit rolled();
}
