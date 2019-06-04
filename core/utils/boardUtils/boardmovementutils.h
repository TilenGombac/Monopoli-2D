#ifndef BOARDMOVEMENTUTILS_H
#define BOARDMOVEMENTUTILS_H

class Player;
class Field;

class BoardMovementUtils
{
public:
	static int getTargetId(Player *player, const int &diceValue);
	static bool move(Player *player, Field *target);

};

#endif // BOARDMOVEMENTUTILS_H
