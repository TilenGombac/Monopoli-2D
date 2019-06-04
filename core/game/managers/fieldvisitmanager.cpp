#include "fieldvisitmanager.h"

#include "core/game/entity/dice.h"
#include "core/game/board/board.h"
#include "core/game/entity/token.h"
#include "core/game/entity/field.h"
#include "core/game/player/player.h"
#include "core/utils/gameUtils/gameutils.h"
#include "core/game/managers/boardmanager.h"
#include "core/game/entity/cards/chancecard.h"
#include "core/game/entity/fields/propertyfield.h"
#include "core/game/entity/fields/possessablefield.h"
#include "core/utils/boardUtils/boardmovementutils.h"
#include "core/game/entity/cards/communitychestcard.h"

FieldVisitManager::FieldVisitManager(BoardManager *setBoardManager)
	: QObject()
{
	boardManager = setBoardManager;

	for(Player *player : boardManager->getBoard()->getPlayers())
	{
		connect(player->getToken(), SIGNAL(reachedTarget()),
					this, SLOT(handleVisit()));
	}
}

void FieldVisitManager::handleVisit()
{
	Board *board = boardManager->getBoard();

	Player *player = boardManager->getPlayer(board->getPlayerTurn());

	Field *field = player->getToken()->getField();

	ActionType actionType = field->getAction().getActionType();
	float actionValue     = field->getAction().getActionValue();

	if(actionType == ActionType::CARD)
	{
		Card *card;
		if(actionValue == Action::COMMUNITY_CHEST)
		{
			card = boardManager->getCommunityChestCard();
		}
		else
		{
			card = boardManager->getChanceCard();
		}

		SpecialAction specialAction = card->getSpecialAction();

		if(specialAction != SpecialAction::NONE)
		{
			if(specialAction == SpecialAction::BIRTHDAY)
			{
				actionValue = card->getAction().getActionValue();

				for(Player *p : board->getPlayers())
				{
					if(p->getId() != player->getId())
					{
						p->requestPayment(actionValue);
						player->deposit(actionValue);
					}
				}

				card->show();

				return;
			}
			else if(specialAction == SpecialAction::CHAIRMAN)
			{
				float amount = 0;

				actionValue = card->getAction().getActionValue();

				for(Player *p : board->getPlayers())
				{
					if(p->getId() != player->getId())
					{
						amount += actionValue;

						p->deposit(actionValue);
					}
				}

				player->requestPayment(amount);

				card->show();

				return;
			}
			else if(specialAction == SpecialAction::REPAIRS)
			{
				float houseRepairCost, hotelRepairCost;

				float totalRepairCost = 0;

				if(card->getCardType() == CardType::COMMUNITY_CHEST)
				{
					houseRepairCost = 40;
					hotelRepairCost = 115;
				}
				else
				{
					houseRepairCost = 25;
					hotelRepairCost = 100;
				}

				for(Field *f : board->getFields())
				{
					if(f->getFieldType() == FieldType::PROPERTY_FIELD)
					{
						PropertyField *tmp = dynamic_cast<PropertyField*> (f);

						int buildings = (int)tmp->getBuildings();

						if(buildings == 5)
						{
							totalRepairCost += hotelRepairCost;
						}
						else
						{
							totalRepairCost += buildings * houseRepairCost;
						}
					}
				}

				player->requestPayment(totalRepairCost);

				card->show();

				return;
			}
		}

		Action cardAction = card->getAction();

		actionType = cardAction.getActionType();
		actionValue = cardAction.getActionValue();

		card->show();
	}

	if(actionType == ActionType::PAY)
	{
		if(field->getPossessable())
		{
			PossessableField *tmp = dynamic_cast<PossessableField*> (field);

			// Do not collect rent on own properties
			if(tmp->getOwnerId() == player->getId())
				return;

			// Do not collect rent on mortgaged properties
			if(tmp->getMortgaged())
				return;

			// "Utilities" fields
			if((tmp->getId() == 12) || (tmp->getId() == 28))
			{
				float diceMultiply = tmp->getRent(true);

				Dice *dice = boardManager->getBoard()->getDice();

				int diceValue = dice->getFirst() + dice->getSecond();

				player->requestPayment(diceMultiply * diceValue);

				boardManager->getPlayer(tmp->getOwnerId())->deposit(diceMultiply * diceValue);

				return;
			}

			boardManager->getPlayer(tmp->getOwnerId())->deposit(actionValue);
		}

		player->requestPayment(actionValue);

		return;
	}

	if(actionType == ActionType::COLLECT)
	{
		player->deposit(actionValue);

		return;
	}

	if(actionType == ActionType::JAIL)
	{
		if(actionValue == Action::GO_TO_JAIL)
			boardManager->sendToJail(player);
		else
			player->leaveJail();

		return;
	}

	if(actionType == ActionType::MOVE)
	{
		int targetId;
		if(actionValue < 0)
		{
			targetId = (int)((40 + actionValue) + field->getId()) % 40;

			player->getToken()->setField(boardManager->getField(targetId));
		}
		else
		{
			targetId = actionValue;

			bool passedGo = BoardMovementUtils::move(player,
											boardManager->getField(targetId));

			GameUtils::handlePassGo(player, passedGo);
		}
	}
}
