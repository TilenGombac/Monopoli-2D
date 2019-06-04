#ifndef GAMEUTILS_H
#define GAMEUTILS_H

class Field;
class Player;
class BoardManager;

class GameUtils
{
public:
	static void handlePassGo(Player* player, const bool &passedGo);
	static void build(Field *field, Player *player);
	static void destruct(Field *field, Player *player);

	static void addBuilding(Field* field);
	static void removeBuilding(Field* field);

};

#endif // GAMEUTILS_H
