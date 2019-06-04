#ifndef AUTOMOBILETOKEN_H
#define AUTOMOBILETOKEN_H

#include "core/game/entity/token.h"

class AutomobileToken : public Token
{
public:
	AutomobileToken(Field *setField, QWidget *parent = 0);

	void move(Field *targetField);
};

#endif // AUTOMOBILETOKEN_H
