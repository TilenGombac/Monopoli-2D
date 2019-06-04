#ifndef SCOTTISHTERRIERTOKEN_H
#define SCOTTISHTERRIERTOKEN_H

#include "core/game/entity/token.h"

class ScottishTerrierToken : public Token
{
public:
	ScottishTerrierToken(Field *setField, QWidget *parent = 0);

	void move(Field *targetField);
};

#endif // SCOTTISHTERRIERTOKEN_H
