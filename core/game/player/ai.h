#ifndef AI_H
#define AI_H

#include <QObject>

#include "core/game/player/player.h"

class BoardManager;
class PlayerActionManager;

class AI : public Player
{
	Q_OBJECT

	BoardManager *boardManager;
	PlayerActionManager *actionManager;

public:
	AI(Token *setToken);
	AI(Token *setToken, const QString &setName);

	void setBoardManager(BoardManager *setBoardManager);

	void tryToBuild();

	void suggestTrade(Offer *offer);

	void requestPayment(const float &amount);

	void distributeProperty(float debt);

public slots:
	void startTurn();
	void performActions();

};

#endif // AI_H
