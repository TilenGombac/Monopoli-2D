#ifndef CHANCECARD_H
#define CHANCECARD_H

#include "core/game/entity/card.h"

class ChanceCard : public Card
{
public:
	ChanceCard(const int &id, QWidget* parent = 0);

protected:
	void paintEvent(QPaintEvent* event);

};

#endif // CHANCECARD_H
