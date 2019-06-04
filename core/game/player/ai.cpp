#include "ai.h"

#include <QTimer>

#include "core/game/board/board.h"
#include "core/game/trade/offer.h"
#include "core/game/entity/token.h"
#include "core/game/managers/boardmanager.h"
#include "core/utils/fieldUtils/fieldutils.h"
#include "core/game/managers/playeractionmanager.h"
#include "core/game/entity/fields/possessablefield.h"
#include "core/utils/fieldUtils/fieldmortgageutils.h"

AI::AI(Token *setToken)
	: Player(setToken)
{
	playerType = PlayerType::AI;
}

AI::AI(Token *setToken, const QString &setName)
	: Player(setToken, setName)
{
	playerType = PlayerType::AI;
}

void AI::setBoardManager(BoardManager *setBoardManager)
{
	boardManager = setBoardManager;

	actionManager = new PlayerActionManager(boardManager, this);

	actionManager->setShowMessages(false);

	connect(this->getToken(), SIGNAL(reachedTarget()),
				this, SLOT(performActions()));
}

void AI::tryToBuild()
{
	for(Field *tmp : fields)
	{
		actionManager->setSelectedField(tmp);
		actionManager->build();
	}

	actionManager->setSelectedField(getToken()->getField());
}

void AI::startTurn()
{
	if(!(actionManager->getDiceThrown())
			&& (boardManager->getBoard()->getPlayerTurn() == this->getId()))
	{
		QTimer *timer = new QTimer(this);

		connect(timer, SIGNAL(timeout()), timer, SLOT(stop()));
		connect(timer, SIGNAL(timeout()), actionManager, SLOT(rollDice()));

		timer->start(800); // Adding some reaction time to the AI
	}
}

void AI::performActions()
{
	if(!(this->getToken()->getMoving()))
	{
		if(this->getJailed() == true)
		{
			if(this->getJailedTurnsLeft() > 1)
			{
				actionManager->endTurn();

				return;
			}

			if(this->withdraw(50))
			{
				this->leaveJail();

				if(actionManager->getDiceThrown())
				{
					actionManager->endTurn();
				}
				else
				{
					this->startTurn();

					return;
				}
			}
			else
			{
				actionManager->endTurn();

				return;
			}
		}

		actionManager->buyField();

		this->tryToBuild();

		if(actionManager->getDiceThrown())
		{
			actionManager->endTurn();
		}
		else
		{
			this->startTurn();
		}
	}
}

void AI::suggestTrade(Offer *offer)
{
	OfferPair providedPair = offer->getProvided();
	OfferPair returnedPair = offer->getReturned();

	float providedValue = providedPair.getAmount();
	float returnedValue = returnedPair.getAmount();

	QVector<PossessableField*> providedFields = providedPair.getFields();
	QVector<PossessableField*> returnedFields = returnedPair.getFields();

	for(PossessableField *tmp : providedFields)
	{
		providedValue += FieldUtils::getFieldValue(tmp);
	}

	for(PossessableField *tmp : returnedFields)
	{
		returnedValue += FieldUtils::getFieldValue(tmp);
	}

	if(providedValue > returnedValue)
	{
		offer->acceptOffer();
	}
	else
	{
		offer->denyOffer();
	}
}

void AI::requestPayment(const float &amount)
{
	if(this->withdraw(amount))
	{
		emit paymentRequestSuccessful(amount);
	}
	else
	{
		if(this->getNetWorth() < amount)
		{
			this->distributeProperty(amount);

			emit leaveGame(this);
		}
		else
		{
			for(Field* f : fields)
			{
				this->mortgage(f);

				if(this->withdraw(amount))
				{
					emit paymentRequestSuccessful(amount);

					return;
				}
			}

			this->distributeProperty(amount);
		}
	}
}

void AI::distributeProperty(float debt)
{
	Field* currentField = this->getToken()->getField();

	// Probably standing on a possessablefield and are in trouble due
	//	to rent cost
	if(currentField->getPossessable())
	{
		PossessableField* tmp = dynamic_cast<PossessableField*> (currentField);

		int ownerId = tmp->getOwnerId();

		if((ownerId != -1) && (ownerId != this->getId()))
		{
			Player* creditor = boardManager->getPlayer(ownerId);

			float funds = this->getAccountBalance();

			debt -= funds;

			this->transfer(funds, creditor);

			for(Field* f : fields)
			{
				debt -= FieldUtils::getFieldValue(f);

				this->transfer((PossessableField*)f, creditor);

				if(debt <= 0)
				{
					return;
				}
			}
		}
	}
	else
	{
		for(Field* f : fields)
		{
			PossessableField* tmp = (PossessableField*)f;
			tmp->setOwnerId(-1);

			debt -= FieldUtils::getFieldValue(f);

			this->removeField(f);

			if(debt <= 0)
			{
				return;
			}
		}
	}
}
