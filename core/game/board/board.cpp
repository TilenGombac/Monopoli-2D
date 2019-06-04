#include "board.h"

#include "core/game/entity/fields/cardfield.h"
#include "core/game/entity/fields/companyfield.h"
#include "core/game/entity/fields/cornerfield.h"
#include "core/game/entity/fields/propertyfield.h"
#include "core/game/entity/fields/taxfield.h"

#include "core/game/player/ai.h"
#include "core/game/player/localplayer.h"
#include "core/game/player/networkplayer.h"

#include "core/game/entity/cards/chancecard.h"
#include "core/game/entity/cards/communitychestcard.h"

#include "core/game/entity/dice.h"

Board::Board()
	: QObject()
{
	dice = new Dice();

	this->setPlayerTurn(-1);
}

Board::~Board()
{
	dice->deleteLater();
}

void Board::setCards(const QVector<Card *> &setCards)
{
	cards = setCards;
}

void Board::setFields(const QVector<Field *> &setFields)
{
	fields = setFields;
}

void Board::setPlayers(const QVector<Player *> &setPlayers)
{
	players = setPlayers;
}

void Board::setDice(Dice *setDice)
{
	dice = setDice;
}

void Board::setPlayerTurn(const int &setPlayerTurn)
{
	playerTurn = setPlayerTurn;

	for(Player *player : players)
	{
		if((player->getJailed()) && (player->getId() == playerTurn))
		{
			if(player->getJailedTurnsLeft() > 0)
			{
				player->jailTick();

				break;
			}
			else
			{
				player->leaveJail(true);

				break;
			}
		}
	}

	this->resetSpeedingCounter();

	emit playerTurnChanged();
}

void Board::addSpeeding()
{
	speedingCounter++;
}

void Board::setSpeeding(const int &setSpeeding)
{
	speedingCounter = setSpeeding;
}

QVector<Card*> Board::getCards()
{
	return cards;
}

QVector<Field*> Board::getFields()
{
	return fields;
}

QVector<Player*> Board::getPlayers()
{
	return players;
}

Dice* Board::getDice()
{
	return dice;
}

int Board::getPlayerTurn()
{
	return playerTurn;
}

int Board::getSpeedingCounter()
{
	return speedingCounter;
}

void Board::removePlayer(Player* player)
{
	players.removeAll(player);
}

void Board::resetSpeedingCounter()
{
	speedingCounter = 0;
}
