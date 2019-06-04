#ifndef THIMBLETOKEN_H
#define THIMBLETOKEN_H

#include "core/game/entity/token.h"

class ThimbleToken : public Token
{
public:
	ThimbleToken(Field *setField, QWidget *parent = 0);

	void move(Field *targetField);
};

#endif // THIMBLETOKEN_H
