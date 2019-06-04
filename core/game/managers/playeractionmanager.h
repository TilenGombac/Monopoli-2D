#ifndef PLAYERACTIONMANAGER_H
#define PLAYERACTIONMANAGER_H

#include <QObject>
#include <QVector>

class Field;
class Player;
class BoardManager;

class PlayerActionManager : public QObject
{
	Q_OBJECT

	BoardManager *boardManager;
	Player *player;
	Field *selectedField;

	// Don't want messages poping up when AI tries to do something it can't
	bool showMessages;

	bool tradeInProgress;

	bool diceThrown;

public:
	PlayerActionManager(BoardManager *manager, Player *setPlayer);
	void setShowMessages(const bool &setShowMessages);

	void setSelectedField(Field *field);

	bool getTradeInProgress();

	bool getDiceThrown();

public slots:
	void rollDice();
	void mortgageField();
	void unmortgageField();
	void endTurn();
	void build();
	void destruct();
	void buyField();
	void startTradeSetup();
	void stopTrade();

signals:
	void actionPerformed();

};

#endif // PLAYERACTIONMANAGER_H
