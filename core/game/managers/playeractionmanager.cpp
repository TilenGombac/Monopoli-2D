#include "playeractionmanager.h"

#include "core/game/board/board.h"
#include "core/game/entity/dice.h"
#include "core/game/entity/token.h"
#include "core/game/entity/field.h"
#include "core/game/player/player.h"
#include "core/game/trade/tradesetup.h"
#include "core/utils/gameUtils/gameutils.h"
#include "core/game/managers/boardmanager.h"
#include "core/utils/fieldUtils/fieldutils.h"
#include "core/utils/errorUtils/errorhandler.h"
#include "core/game/entity/fields/companyfield.h"
#include "core/game/entity/fields/propertyfield.h"
#include "core/utils/fieldUtils/fieldbuildingutils.h"
#include "core/utils/boardUtils/boardmovementutils.h"
#include "core/utils/fieldUtils/possessablefieldutils.h"

PlayerActionManager::PlayerActionManager(BoardManager *manager,
											Player *setPlayer)
	: QObject()
{
	boardManager = manager;
	player       = setPlayer;

	selectedField = player->getToken()->getField();

	showMessages = true;
	diceThrown   = false;

	tradeInProgress = false;
}

void PlayerActionManager::setShowMessages(const bool &setShowMessages)
{
	showMessages = setShowMessages;
}

void PlayerActionManager::setSelectedField(Field *field)
{
	selectedField = field;
}

bool PlayerActionManager::getTradeInProgress()
{
	return tradeInProgress;
}

bool PlayerActionManager::getDiceThrown()
{
	return diceThrown;
}

void PlayerActionManager::rollDice()
{
	Dice *dice = boardManager->getBoard()->getDice();
	dice->roll();

	if(dice->getFirst() == dice->getSecond())
	{
		diceThrown = false;
		boardManager->getBoard()->addSpeeding();

		if(boardManager->getBoard()->getSpeedingCounter() >= 3)
		{
			player->sendToJail();
			player->getToken()->setField(boardManager->getField(10));

			diceThrown = true;

			player->getToken()->onTargetReached();

			emit actionPerformed();

			return;
		}

		if(player->getJailed())
		{
			player->leaveJail();
		}
	}
	else
	{
		diceThrown = true;
	}

	if(player->getJailed() == false)
	{
		int diceValue = dice->getFirst() + dice->getSecond();
		int targetId  = BoardMovementUtils::getTargetId(player, diceValue);

		Field *target = boardManager->getField(targetId);

		bool passedGo = BoardMovementUtils::move(player, target);

		GameUtils::handlePassGo(player, passedGo);
	}
	else
	{
		player->getToken()->onTargetReached();
	}

	emit actionPerformed();
}

void PlayerActionManager::mortgageField()
{
	Field *field = selectedField;

	if(!(player->mortgage(field)) && (showMessages))
	{
		ErrorHandler::message("Napaka",
								"Hipotekarnega kredita na tem polju"
									" žal ne morate vzeti.");
	}
	else
	{
		// This field can be mortgaged => Possessable Field

		QVector<Field*> fields =
				boardManager->getBoard()->getFields();

		PossessableField *tmp = dynamic_cast<PossessableField*> (field);

		int groupCount =
				PossessableFieldUtils::fieldsInGroup(fields,
												tmp->getGroupId(),
													player->getId());

		// Multiply_1X is represented by 0, 4X by 3...
		GroupMultiply multiply = (GroupMultiply)(groupCount - 1);

		PossessableFieldUtils::setMultiply(fields, tmp->getGroupId(),
											player->getId(), multiply);

		Action action;

		QVector<PossessableField*> groupFields;
		groupFields = PossessableFieldUtils::getGroupFields(fields,
															tmp->getGroupId());

		for(PossessableField *f : groupFields)
		{
			if(f->getOwnerId() == player->getId())
			{
				// false -> not all the fields in this group are owned, no
				// double rent
				action.setPayAction(f->getRent(false));

				f->setAction(action);
			}
		}
	}

	emit actionPerformed();
}

void PlayerActionManager::unmortgageField()
{
	Field *field = selectedField;



	if(!(player->unmortgage(field)) && (showMessages))
	{
		ErrorHandler::message("Napaka",
								"Žal nimate dovolj denarja za odplačilo"
									" tega hipotekarnega kredita.");
	}
	else
	{
		// This field can be unmortgaged => Possessable Field

		QVector<Field*> fields =
				boardManager->getBoard()->getFields();

		PossessableField *tmp = dynamic_cast<PossessableField*> (field);

		int groupCount =
				PossessableFieldUtils::fieldsInGroup(fields,
														tmp->getGroupId(),
															player->getId());

		int allGroupCount =
				PossessableFieldUtils::fieldsInGroup(fields,
													 tmp->getGroupId());
		bool doubleRent = false;

		if(allGroupCount == groupCount)
		{
			doubleRent = true;
		}

		// Multiply_1X is represented by 0, 4X by 3...
		GroupMultiply multiply = (GroupMultiply)(groupCount - 1);

		PossessableFieldUtils::setMultiply(fields, tmp->getGroupId(),
											player->getId(), multiply);

		Action action;

		QVector<PossessableField*> groupFields;
		groupFields =
				PossessableFieldUtils::getGroupFields(fields,
														tmp->getGroupId());

		for(PossessableField *f : groupFields)
		{
			if(f->getOwnerId() == player->getId())
			{
				action.setPayAction(f->getRent(doubleRent));

				f->setAction(action);
			}
		}
	}

	emit actionPerformed();
}

void PlayerActionManager::endTurn()
{
	if(diceThrown)
	{
		diceThrown = false;

		boardManager->setNextPlayerTurn();

		emit actionPerformed();
	}
}

void PlayerActionManager::build()
{
	Field *field = selectedField;

	if(FieldBuildingUtils::canBuild(boardManager->getBoard()->getFields(),
							field, player->getId()))
	{
		GameUtils::build(field, player);
	}

	emit actionPerformed();
}

void PlayerActionManager::destruct()
{
	Field *field = selectedField;

	if(FieldBuildingUtils::canDestruct(boardManager->getBoard()->getFields(),
									   field, player->getId()))
	{
		GameUtils::destruct(field, player);
	}

	emit actionPerformed();
}

void PlayerActionManager::buyField()
{
	Field *field = player->getToken()->getField();

	if(FieldUtils::canPurchase(field))
	{
		if(!(player->purchase(field)) && (showMessages))
		{
			ErrorHandler::message("Napaka",
								  "Tega polja žal ne morate kupiti.");
		}
		else
		{
			// This field can be purchased => Possessable Field

			QVector<Field*> fields =
					boardManager->getBoard()->getFields();

			PossessableField *tmp = dynamic_cast<PossessableField*> (field);

			int groupCount =
					PossessableFieldUtils::fieldsInGroup(fields,
												tmp->getGroupId(),
													player->getId());

			int allGroupCount =
					PossessableFieldUtils::fieldsInGroup(fields,
														 tmp->getGroupId());
			bool doubleRent = false;

			if(allGroupCount == groupCount)
			{
				doubleRent = true;
			}

			// Multiply_1X is represented by 0, 4X by 3...
			GroupMultiply multiply = (GroupMultiply)(groupCount - 1);

			PossessableFieldUtils::setMultiply(fields, tmp->getGroupId(),
												player->getId(), multiply);

			Action action;

			QVector<PossessableField*> groupFields;
			groupFields =
					PossessableFieldUtils::getGroupFields(fields,
															tmp->getGroupId());

			for(PossessableField *f : groupFields)
			{
				if(f->getOwnerId() == player->getId())
				{
					action.setPayAction(f->getRent(doubleRent));

					f->setAction(action);
				}
			}
		}
	}

	emit actionPerformed();
}

void PlayerActionManager::startTradeSetup()
{
	if(!tradeInProgress)
	{
		tradeInProgress = true;

		TradeSetup *tmp = new TradeSetup(boardManager, player);

		connect(tmp, SIGNAL(offerSent()),
					this, SLOT(stopTrade()));

		connect(tmp, SIGNAL(destroyed(QObject*)),
					this, SLOT(stopTrade()));

		for(Field *f : boardManager->getBoard()->getFields())
		{
			if(f->getPossessable())
			{
				connect(f, SIGNAL(fieldSelected(Field*)),
							tmp, SLOT(addField(Field*)));
			}
		}

		tmp->hide();
	}

	emit actionPerformed();
}

void PlayerActionManager::stopTrade()
{
	tradeInProgress = false;

	emit actionPerformed();
}
