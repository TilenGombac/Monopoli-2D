#include "serverplayeractionmanager.h"

#include "core/game/board/board.h"
#include "core/game/entity/dice.h"
#include "core/game/entity/token.h"
#include "core/utils/gameUtils/gameutils.h"
#include "core/game/managers/boardmanager.h"
#include "core/utils/fieldUtils/fieldutils.h"
#include "core/game/managers/fieldvisitmanager.h"
#include "core/utils/boardUtils/boardmovementutils.h"
#include "core/utils/fieldUtils/fieldbuildingutils.h"
#include "core/utils/fieldUtils/possessablefieldutils.h"

#include "network/packets/buypacket.h"
#include "network/packets/jailpacket.h"
#include "network/packets/movepacket.h"
#include "network/packets/errorpacket.h"
#include "network/packets/depositpacket.h"
#include "network/packets/setturnpacket.h"
#include "network/packets/withdrawpacket.h"
#include "network/packets/updatedicepacket.h"
#include "network/packets/playeractionpacket.h"
#include "network/packets/notificationpacket.h"
#include "network/packets/requestpaymentpacket.h"
#include "network/packets/updatedashboardpacket.h"

ServerPlayerActionManager::ServerPlayerActionManager(QVector<ServerClient*> clients,
														BoardManager* bm,
															QObject* parent)
	: QObject(parent)
{
	this->serverClients = clients;
	this->boardManager  = bm;

	pendingPayment = false;
	diceRolled     = false;
	debt           = 0;

	visitManager = new FieldVisitManager(boardManager);

	for(Player* player : boardManager->getBoard()->getPlayers())
	{
		connect(player, SIGNAL(paymentRequestFailed(float)),
					this, SLOT(onPaymentFailedReceived(float)));

		connect(player, SIGNAL(paymentRequestSuccessful(float)),
					this, SLOT(onPaymentSuccessReceived(float)));
	}

	for(ServerClient* client : this->serverClients)
	{
		connect(client, SIGNAL(diceRolled(int)),
					this, SLOT(onDiceRolledReceived(int)));

		connect(client, SIGNAL(endTurn(int)),
					this, SLOT(onEndTurnReceived(int)));

		connect(client, SIGNAL(buyRequest(int)),
					this, SLOT(onBuyReceived(int)));

		connect(client, SIGNAL(unmortgageRequest(int,int)),
					this, SLOT(onUnmortgageReceived(int,int)));

		connect(client, SIGNAL(mortgageRequest(int,int)),
					this, SLOT(onMortgageReceived(int,int)));

		connect(client, SIGNAL(buildRequest(int,int)),
					this, SLOT(onBuildReceived(int,int)));

		connect(client, SIGNAL(destructRequest(int,int)),
					this, SLOT(onDestructReceived(int,int)));

		connect(client, SIGNAL(leaveJailRequest(int)),
					this, SLOT(onLeaveJailReceived(int)));
	}
}

ServerPlayerActionManager::~ServerPlayerActionManager()
{
	visitManager->deleteLater();
}

QTcpSocket* ServerPlayerActionManager::getPlayerSocket(int playerId)
{
	for(ServerClient* client : serverClients)
	{
		Player* tmp = client->getPlayer();

		if(boardManager->getBoard()->getPlayers().indexOf(tmp) != -1)
		{
			if(tmp->getId() == playerId)
			{
				return client->getSocket();
			}
		}
	}

	return nullptr;
}

void ServerPlayerActionManager::sendAvailableActions()
{
	int playerTurn = boardManager->getBoard()->getPlayerTurn();

	Player* player = boardManager->getPlayer(playerTurn);

	QVector<PlayerAction> actions;

	if(!pendingPayment)
	{
		if(diceRolled)
		{
			actions.append(PlayerAction::END_TURN);
		}
		else
		{
			actions.append(PlayerAction::ROLL_DICE);
		}
	}

	UpdateDashboardPacket packet(actions);

	this->getPlayerSocket(player->getId())->write(packet.getData(true));
}

void ServerPlayerActionManager::onDiceRolledReceived(int playerId)
{
	Board* board = boardManager->getBoard();
	if(board->getPlayerTurn() == playerId)
	{
		if(pendingPayment)
		{
			ErrorPacket errorPacket(ErrorCode::GENERAL,
										"Najprej moraš odplačati dolgove.");

			this->getPlayerSocket(playerId)->write(errorPacket.getData(true));

			return;
		}

		if(diceRolled)
		{
			ErrorPacket errorPacket(ErrorCode::GENERAL,
										"Met kock to potezo ni več na voljo.");

			this->getPlayerSocket(playerId)->write(errorPacket.getData(true));

			return;
		}

		Dice* dice = board->getDice();

		dice->roll();

		diceRolled = true;

		// Send updated dice immediatelly, perform other actions afterwards
		UpdateDicePacket dicePacket(dice->getFirst(), dice->getSecond());
		for(Client* client : serverClients)
		{
			client->getSocket()->write(dicePacket.getData(true));
		}

		Player* player = boardManager->getPlayer(playerId);

		if(dice->getFirst() == dice->getSecond())
		{
			diceRolled = false;

			boardManager->getBoard()->addSpeeding();

			// Send to jail if it's the third double dice this turn
			if(boardManager->getBoard()->getSpeedingCounter() >= 3)
			{
				player->sendToJail();
				player->getToken()->setField(boardManager->getField(10));

				JailPacket jailPacket(playerId, JailAction::GO_TO_JAIL);

				for(Client* client : serverClients)
				{
					client->getSocket()->write(jailPacket.getData(true));
				}

				return;
			}

			// If the player is already in jail, release him
			// (and move him, further down the line)
			if(player->getJailed())
			{
				JailPacket jailPacket(playerId, JailAction::LEAVE_JAIL);

				for(Client* client : serverClients)
				{
					client->getSocket()->write(jailPacket.getData(true));
				}

				player->leaveJail();
			}
		}

		if(player->getJailed() == false)
		{
			int diceValue = dice->getFirst() + dice->getSecond();
			int targetId  = BoardMovementUtils::getTargetId(player, diceValue);

			Field* target = boardManager->getField(targetId);

			bool passedGo = BoardMovementUtils::move(player, target);

			DepositPacket depositPacket(playerId, 200);

			if(passedGo)
			{
				player->deposit(depositPacket.getAmount());
			}

			MovePacket movePacket(playerId, targetId);

			for(Client* client : serverClients)
			{
				client->getSocket()->write(movePacket.getData(true));

				if(passedGo)
				{
					client->getSocket()->write(depositPacket.getData(true));
				}
			}
		}
	}

	this->sendAvailableActions();
}

void ServerPlayerActionManager::onEndTurnReceived(int playerId)
{	
	if(boardManager->getBoard()->getPlayerTurn() == playerId)
	{
		if(pendingPayment)
		{
			ErrorPacket errorPacket(ErrorCode::GENERAL,
										"Najprej moraš odplačati dolgove.");

			this->getPlayerSocket(playerId)->write(errorPacket.getData(true));

			return;
		}

		if(!diceRolled)
		{
			ErrorPacket errorPacket(ErrorCode::GENERAL,
										"Najprej vrzi kocke.");

			this->getPlayerSocket(playerId)->write(errorPacket.getData(true));

			return;
		}

		diceRolled = false;

		boardManager->setNextPlayerTurn();

		int playerTurn = boardManager->getBoard()->getPlayerTurn();

		SetTurnPacket packet(playerTurn);

		for(Client* client : serverClients)
		{
			client->getSocket()->write(packet.getData(true));
		}
	}

	this->sendAvailableActions();
}

void ServerPlayerActionManager::onBuyReceived(int playerId)
{
	if(pendingPayment)
	{
		// Buying is not allowed while pending payment

		return;
	}

	if(boardManager->getBoard()->getPlayerTurn() == playerId)
	{
		Player* player = boardManager->getPlayer(playerId);

		Field* field = player->getToken()->getField();

		if(FieldUtils::canPurchase(field))
		{
			if(!(player->purchase(field)))
			{
				// Player can't buy the field (not enough funds)

				return;
			}

			BuyPacket packet(playerId);

			for(Client* client : serverClients)
			{
				client->getSocket()->write(packet.getData(true));
			}

			QVector<Field*> fields = boardManager->getBoard()->getFields();

			PossessableField* tmp = dynamic_cast<PossessableField*> (field);

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

			groupFields = PossessableFieldUtils::getGroupFields(fields,
																tmp->getGroupId());

			for(PossessableField* f : groupFields)
			{
				if(f->getOwnerId() == player->getId())
				{
					action.setPayAction(f->getRent(doubleRent));

					f->setAction(action);
				}
			}
		}
	}

	this->sendAvailableActions();
}

void ServerPlayerActionManager::onUnmortgageReceived(int playerId,
														int fieldId)
{
	if(pendingPayment)
	{
		// Unmortgaging is not allowed while pending payment

		return;
	}

	if(boardManager->getBoard()->getPlayerTurn() == playerId)
	{
		Player* player = boardManager->getPlayer(playerId);

		Field* field = boardManager->getField(fieldId);

		if(player->unmortgage(field))
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

			PlayerActionPacket actionPacket(playerId, fieldId,
												MP_PlayerAction::UNMORTGAGE);

			for(Client* client : serverClients)
			{
				client->getSocket()->write(actionPacket.getData(true));
			}
		}
	}

	this->sendAvailableActions();
}

void ServerPlayerActionManager::onMortgageReceived(int playerId,
													int fieldId)
{
	if(boardManager->getBoard()->getPlayerTurn() == playerId)
	{
		Player* player = boardManager->getPlayer(playerId);

		Field* field = boardManager->getField(fieldId);

		if(player->mortgage(field))
		{
			// This field can be mortgaged => Possessable Field

			QVector<Field*> fields = boardManager->getBoard()->getFields();

			PossessableField* tmp = (PossessableField*)field;

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

			for(PossessableField* f : groupFields)
			{
				if(f->getOwnerId() == player->getId())
				{
					// false -> not all the fields in this group are owned, no
					// double rent
					action.setPayAction(f->getRent(false));

					f->setAction(action);
				}
			}

			PlayerActionPacket actionPacket(playerId, fieldId,
												MP_PlayerAction::MORTGAGE);

			for(Client* client : serverClients)
			{
				client->getSocket()->write(actionPacket.getData(true));
			}

			if(pendingPayment)
			{
				player->requestPayment(debt);
			}
		}
	}

	this->sendAvailableActions();
}

void ServerPlayerActionManager::onBuildReceived(int playerId, int fieldId)
{
	if(pendingPayment)
	{
		// Building is not allowed while pending payment

		return;
	}

	if(boardManager->getBoard()->getPlayerTurn() == playerId)
	{
		QVector<Field*> fields = boardManager->getBoard()->getFields();

		Field* field = boardManager->getField(fieldId);

		if(FieldBuildingUtils::canBuild(fields, field, playerId))
		{
			Player* player = boardManager->getPlayer(playerId);

			float startingBalance = player->getAccountBalance();

			GameUtils::build(field, player);

			float delta = startingBalance - player->getAccountBalance();

			if(delta > 0)
			{
				WithdrawPacket withdrawPacket(playerId, delta);
				PlayerActionPacket actionPacket(playerId, fieldId,
													MP_PlayerAction::BUILD);

				for(Client* client : serverClients)
				{
					client->getSocket()->write(withdrawPacket.getData(true));
					client->getSocket()->write(actionPacket.getData(true));
				}
			}
			else
			{
				ErrorPacket errorPacket(ErrorCode::GENERAL, "Nimate dovolj sredstev!");

				for(ServerClient* client : serverClients)
				{
					if(client->getPlayer() == player)
					{
						client->getSocket()->write(errorPacket.getData(true));

						break;
					}
				}
			}
		}
	}

	this->sendAvailableActions();
}

void ServerPlayerActionManager::onDestructReceived(int playerId,
													int fieldId)
{
	if(boardManager->getBoard()->getPlayerTurn() == playerId)
	{
		QVector<Field*> fields = boardManager->getBoard()->getFields();

		Field* field = boardManager->getField(fieldId);

		if(FieldBuildingUtils::canDestruct(fields, field, playerId))
		{
			Player* player = boardManager->getPlayer(playerId);

			float startingBalance = player->getAccountBalance();

			GameUtils::destruct(field, player);

			float delta = player->getAccountBalance() - startingBalance;

			DepositPacket depositPacket(playerId, delta);
			PlayerActionPacket actionPacket(playerId, fieldId,
												MP_PlayerAction::DESTRUCT);

			for(Client* client : serverClients)
			{
				client->getSocket()->write(depositPacket.getData(true));
				client->getSocket()->write(actionPacket.getData(true));
			}

			if(pendingPayment)
			{
				player->requestPayment(debt);
			}
		}
	}

	this->sendAvailableActions();
}

void ServerPlayerActionManager::onLeaveJailReceived(int playerId)
{
	if(pendingPayment)
	{
		// Leaving jail is not allowed while pending payment

		return;
	}

	if(boardManager->getBoard()->getPlayerTurn() == playerId)
	{
		Player* player = boardManager->getPlayer(playerId);

		if(player->getJailed())
		{
			if(player->withdraw(50))
			{
				player->leaveJail();

				JailPacket jailPacket(playerId, JailAction::LEAVE_JAIL);
				WithdrawPacket withdrawPacket(playerId, 50);

				for(Client* client : serverClients)
				{
					client->getSocket()->write(jailPacket.getData(true));
					client->getSocket()->write(withdrawPacket.getData(true));
				}
			}
		}
	}

	this->sendAvailableActions();
}

void ServerPlayerActionManager::onPaymentFailedReceived(float amount)
{
	pendingPayment = true;

	debt = amount;

	Player* player = qobject_cast<Player*> (QObject::sender());

	int playerId = player->getId();

	RequestPaymentPacket packet(playerId, amount);

	for(Client* client : serverClients)
	{
		client->getSocket()->write(packet.getData(true));
	}

	this->sendAvailableActions();
}

void ServerPlayerActionManager::onPaymentSuccessReceived(float amount)
{
	Player* player = qobject_cast<Player*> (QObject::sender());

	if(pendingPayment)
	{
		WithdrawPacket withdrawPacket(player->getId(), amount);

		// Deposit the rent to the field's owner
		if(player->getToken()->getField()->getPossessable())
		{
			PossessableField* tmp =
					(PossessableField*)player->getToken()->getField();

			int ownerId = tmp->getOwnerId();

			DepositPacket depositPacket(ownerId, amount);

			for(Client* client : this->serverClients)
			{
				client->getSocket()->write(depositPacket.getData(true));
			}
		}

		for(ServerClient* client : this->serverClients)
		{
			client->getSocket()->write(withdrawPacket.getData(true));

			if(client->getPlayer() == player)
			{
				NotificationPacket msgPacket("Dolgovi uspešno odplačani.",
												NotificationType::INFORMATION);

				client->getSocket()->write(msgPacket.getData(true));
			}
		}

		pendingPayment = false;
		debt = 0;
	}

	// Rent calculation is server side only! Other costs
	// are taken care of client-side already
	if(player->getToken()->getField()->getPossessable())
	{
		PossessableField* tmp =
				(PossessableField*)player->getToken()->getField();

		int ownerId  = tmp->getOwnerId();
		int playerId = player->getId();

		WithdrawPacket withdrawPacket(playerId, amount);
		DepositPacket depositPacket(ownerId, amount);

		for(Client* client : serverClients)
		{
			client->getSocket()->write(withdrawPacket.getData(true));
			client->getSocket()->write(depositPacket.getData(true));
		}
	}


	this->sendAvailableActions();
}

void ServerPlayerActionManager::onOfferAccepted()
{
	if(pendingPayment)
	{
		int playerTurn = this->boardManager->getBoard()->getPlayerTurn();

		Player* debtor = this->boardManager->getPlayer(playerTurn);

		debtor->requestPayment(debt);
	}
}

void ServerPlayerActionManager::transferFunds(Player *player)
{
	// Transfer all the player's fields back to the bank
	// If the player is in debt, transfer player networth to the creditor
	if(boardManager->getBoard()->getPlayerTurn() == player->getId())
	{
		if(pendingPayment && player->getToken()->getField()->getPossessable())
		{
			PossessableField* tmp = (PossessableField*)player->getToken()->getField();

			DepositPacket packet(tmp->getOwnerId(), player->getNetWorth());

			boardManager->getPlayer(tmp->getOwnerId())->deposit(player->getNetWorth());

			for(Client* client : this->serverClients)
			{
				client->getSocket()->write(packet.getData(true));
			}
		}
	}

	for(Field* field : player->getFields())
	{
		if(field->getPossessable())
		{
			PossessableField* tmp = (PossessableField*)field;

			tmp->setOwnerId(-1);
		}
	}

	pendingPayment = false;

	debt = 0;
}
