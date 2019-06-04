#ifndef HOWITZERTOKEN_H
#define HOWITZERTOKEN_H

#include "core/game/entity/token.h"

class HowitzerToken : public Token
{
public:
	HowitzerToken(Field *setField, QWidget *parent = 0);

	void move(Field *targetField);
};

#endif // HOWITZERTOKEN_H
