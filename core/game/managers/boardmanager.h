#ifndef BOARDMANAGER_H
#define BOARDMANAGER_H

#include <QObject>

#include "core/game/entity/card.h"

class Board;
class Field;
class Player;
class ChanceCard;
class PossessableField;
class CommunityChestCard;

class BoardManager : public QObject
{
	Q_OBJECT

	Board *board;

public:
	BoardManager();
	~BoardManager();

	void setNextPlayerTurn();

	Board* getBoard();

	CommunityChestCard* getCommunityChestCard();
	ChanceCard* getChanceCard();

	Field* getField(const int &fieldId = 0);

	Player* getPlayer(const int &playerId);

	void sendToJail(Player *player);

private:
	Card* getTopCard(const CardType &cardType);

public slots:
	void removePlayer(Player* player);

signals:
	void playerRemoved(Player *player);
	void gameOver(Player* winner);

};

#endif // BOARDMANAGER_H
