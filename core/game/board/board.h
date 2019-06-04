#ifndef BOARD_H
#define BOARD_H

#include <QObject>
#include <QVector>

class Dice;
class Card;
class Field;
class Player;

class Board : public QObject
{
	Q_OBJECT

	QVector<Card*> cards;
	QVector<Field*> fields;
	QVector<Player*> players;

	Dice *dice;

	int playerTurn;

	int speedingCounter;

public:
	Board();
	~Board();

	void setCards(const QVector<Card*> &setCards);
	void setFields(const QVector<Field*> &setFields);
	void setPlayers(const QVector<Player*> &setPlayers);

public slots:
	void setDice(Dice *setDice);

	void setPlayerTurn(const int &setPlayerTurn);

	void addSpeeding();

	void setSpeeding(const int &setSpeeding);

	QVector<Card*> getCards();
	QVector<Field*> getFields();
	QVector<Player*> getPlayers();

	Dice* getDice();

	int getPlayerTurn();

	int getSpeedingCounter();

	void removePlayer(Player* player);

private:
	void resetSpeedingCounter();

signals:
	void playerTurnChanged();

};

#endif // BOARD_H
