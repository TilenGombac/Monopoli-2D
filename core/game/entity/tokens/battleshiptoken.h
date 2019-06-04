#ifndef BATTLESHIPTOKEN_H
#define BATTLESHIPTOKEN_H

#include "core/game/entity/token.h"

class BattleshipToken : public Token
{
public:
	BattleshipToken(Field *setField, QWidget *parent = 0);

	void move(Field *targetField);
};

#endif // BATTLESHIPTOKEN_H
