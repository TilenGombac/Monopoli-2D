#include "boardmanager.h"

#include "core/game/entity/field.h"
#include "core/game/entity/cards/chancecard.h"
#include "core/game/entity/fields/possessablefield.h"
#include "core/game/entity/cards/communitychestcard.h"
#include "core/game/board/board.h"
#include "core/game/entity/token.h"
#include "core/game/player/player.h"
#include "core/game/managers/cardsmanager.h"
#include "core/game/managers/fieldsmanager.h"

#include "core/utils/errorUtils/errorhandler.h"

BoardManager::BoardManager()
	: QObject()
{
	board = new Board();
}

BoardManager::~BoardManager()
{
	qDeleteAll(board->getCards());
	qDeleteAll(board->getFields());
	qDeleteAll(board->getPlayers());
}

void BoardManager::setNextPlayerTurn()
{
	int playerTurn = board->getPlayerTurn();

	// This do-while prevents passing turn to a player who has already left
	do
	{
		int playerCount = board->getPlayers().size();

		playerTurn++;

		playerTurn = playerTurn % playerCount;
	}
	while(getPlayer(playerTurn) == nullptr);

	board->setPlayerTurn(playerTurn);
}

Board* BoardManager::getBoard()
{
	return board;
}

CommunityChestCard* BoardManager::getCommunityChestCard()
{
	Card* card = this->getTopCard(CardType::COMMUNITY_CHEST);

	return dynamic_cast<CommunityChestCard*> (card);
}

ChanceCard* BoardManager::getChanceCard()
{
	Card* card = this->getTopCard(CardType::CHANCE);

	return dynamic_cast<ChanceCard*> (card);
}

Field* BoardManager::getField(const int &fieldId)
{
	for(Field* field : board->getFields())
	{
		if(field->getId() == fieldId)
		{
			return field;
		}
	}

	return nullptr;
}

Player* BoardManager::getPlayer(const int &playerId)
{
	for(Player* player : board->getPlayers())
	{
		if(player->getId() == playerId)
		{
			return player;
		}
	}

	return nullptr;
}

void BoardManager::sendToJail(Player *player)
{
	player->getToken()->setField(this->getField(10));

	if(player->getPlayerType() != PlayerType::NETWORK)
	{
		this->setNextPlayerTurn();
	}

	player->sendToJail();
	player->getToken()->onTargetReached();
}

Card* BoardManager::getTopCard(const CardType &cardType)
{
	QVector<Card*> cards = board->getCards();

	Card *card = nullptr;

	for(int i = 0; i < cards.size(); i++)
	{
		if(cards.at(i)->getCardType() == cardType)
		{
			// Save top card pointer for return
			card = cards.at(i);
			// Remove top card - beggining of vector
			cards.removeAt(i);
			// Put it at the bottom - end of vector in our case
			cards.append(card);

			break;
		}
	}

	// Save the new deck state
	board->setCards(cards);

	return card;
}

void BoardManager::removePlayer(Player *player)
{
	if(player->getId() == board->getPlayerTurn())
		this->setNextPlayerTurn();

	int i = board->getPlayers().indexOf(player);

	if(i != -1)
	{
		board->removePlayer(player);

		emit playerRemoved(player);

		player->getToken()->hide();
	}

	if(board->getPlayers().size() == 1)
	{
		// emit game over signal, with the only remaining
		// player (winner) as the parameter
		emit gameOver(board->getPlayers().at(0));
	}
}
