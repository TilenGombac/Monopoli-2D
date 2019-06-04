#ifndef TOPHATTOKEN_H
#define TOPHATTOKEN_H

#include "core/game/entity/token.h"

class TopHatToken : public Token
{
public:
	TopHatToken(Field *setField, QWidget *parent = 0);

	void move(Field *targetField);
};

#endif // TOPHATTOKEN_H
