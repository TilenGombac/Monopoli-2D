#ifndef COMMUNITYCHESTCARD_H
#define COMMUNITYCHESTCARD_H

#include "core/game/entity/card.h"

class CommunityChestCard : public Card
{
public:
	CommunityChestCard(const int &id, QWidget* parent = 0);

protected:
	void paintEvent(QPaintEvent *event);

};

#endif // COMMUNITYCHESTCARD_H
