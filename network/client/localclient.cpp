#include "localclient.h"

#include "network/packets/buypacket.h"
#include "network/packets/movepacket.h"
#include "network/packets/pingpacket.h"
#include "network/packets/chatpacket.h"
#include "network/packets/jailpacket.h"
#include "network/packets/errorpacket.h"
#include "network/packets/depositpacket.h"
#include "network/packets/connectpacket.h"
#include "network/packets/setturnpacket.h"
#include "network/packets/withdrawpacket.h"
#include "network/packets/serverdownpacket.h"
#include "network/packets/updatedicepacket.h"
#include "network/packets/disconnectpacket.h"
#include "network/packets/updatepingpacket.h"
#include "network/packets/notificationpacket.h"
#include "network/packets/playeractionpacket.h"
#include "network/packets/requestpaymentpacket.h"

#include "core/utils/errorUtils/errorhandler.h"
#include "core/notifications/notificationmanager.h"

LocalClient::LocalClient(QHostAddress serverAddress, QObject *parent)
	: Client(parent)
{
	this->socket = new QTcpSocket(parent);

	this->socket->connectToHost(serverAddress, SERVER_PORT);

	if(this->socket->waitForConnected(5000))
	{
		emit connectionStatus(true);
	}
	else
	{
		emit connectionStatus(false);
	}

	connect(this->socket, SIGNAL(readyRead()),
				this, SLOT(preparePacket()));
}

void LocalClient::processPacket(QByteArray data)
{
	PacketType type = Packet::determineType(data);

	if(type == PacketType::CONNECT)
	{
		ConnectPacket packet(data);

		NetworkPlayer *player = packet.getPlayer();

		if(player != nullptr)
		{
			emit playerConnected(player);
		}

		return;
	}

	if(type == PacketType::DISCONNECT)
	{
		DisconnectPacket packet(data);

		int playerId = packet.getPlayerId();

		emit playerDisconnected(playerId);

		return;
	}

	if(type == PacketType::PING)
	{
		PingPacket packet(data);

		Sender sender = packet.getSender();

		if(sender == Sender::CLIENT)
		{
			this->calculatePing();
		}
		else if(sender == Sender::SERVER)
		{
			this->getSocket()->write(packet.getData(true));
		}

		return;
	}

	if(type == PacketType::ERROR)
	{
		ErrorPacket packet(data);

		ErrorCode errorCode = packet.getErrorCode();

		QString errorMessage = packet.getErrorMessage();

		ErrorHandler::message("Napaka", errorMessage);

		if(errorCode == ErrorCode::PLAYER_EXISTS)
		{
			emit playerExists();
		}
		else if(errorCode == ErrorCode::GAME_ACTIVE)
		{
			emit gameAlreadyInProgress();
		}
		else if(errorCode == ErrorCode::GAME_FULL)
		{
			emit gameFull();
		}

		return;
	}

	if(type == PacketType::START)
	{
		emit gameStarted();

		return;
	}

	if(type == PacketType::STOP)
	{
		emit gameStopped();

		NotificationManager::pushNotification("Konec igre",
												"Igra je bila končana",
												NotificationType::INFORMATION);

		return;
	}

	if(type == PacketType::SERVER_DOWN)
	{
		ServerDownPacket packet(data);

		QString reason = packet.getReason();

		emit serverOffline(reason);

		return;
	}

	if(type == PacketType::CHAT)
	{
		ChatPacket packet(data);

		int senderId = packet.getSenderId();
		QString text = packet.getMessage();

		Message msg(senderId, text);

		emit messageReceived(msg);

		return;
	}

	if(type == PacketType::DEPOSIT)
	{
		DepositPacket packet(data);

		int playerId = packet.getPlayerId();
		float amount = packet.getAmount();

		emit deposit(playerId, amount);

		return;
	}

	if(type == PacketType::WITHDRAW)
	{
		WithdrawPacket packet(data);

		int playerId = packet.getPlayerId();
		float amount = packet.getAmount();

		emit withdraw(playerId, amount);

		return;
	}

	if(type == PacketType::MOVE)
	{
		MovePacket packet(data);

		int playerId = packet.getPlayerId();
		int fieldId  = packet.getFieldId();

		emit move(playerId, fieldId);

		return;
	}

	if(type == PacketType::UPDATE_BOARD)
	{
		UpdateBoardPacket packet(data);

		emit boardUpdated(packet);

		return;
	}

	if(type == PacketType::UPDATE_DICE)
	{
		UpdateDicePacket packet(data);

		int first  = packet.getFirst();
		int second = packet.getSecond();

		emit diceUpdated(first, second);

		return;
	}

	if(type == PacketType::JAIL)
	{
		JailPacket packet(data);

		JailAction jailAction = packet.getAction();

		int playerId = packet.getPlayerId();

		if(jailAction == JailAction::GO_TO_JAIL)
		{
			emit sendToJail(playerId);
		}
		else
		{
			emit leaveJail(playerId);
		}

		return;
	}

	if(type == PacketType::BUY)
	{
		BuyPacket packet(data);

		int playerId = packet.getPlayerId();

		emit fieldBought(playerId);

		return;
	}

	if(type == PacketType::SET_TURN)
	{
		SetTurnPacket packet(data);

		int playerId = packet.getPlayerId();

		emit turnChanged(playerId);

		return;
	}

	if(type == PacketType::PLAYER_ACTION)
	{
		PlayerActionPacket packet(data);

		int playerId = packet.getPlayerId();
		int fieldId  = packet.getFieldId();

		MP_PlayerAction action = packet.getPlayerAction();

		if(action == MP_PlayerAction::UNMORTGAGE)
		{
			emit unmortgage(playerId, fieldId);
		}
		else if(action == MP_PlayerAction::MORTGAGE)
		{
			emit mortgage(playerId, fieldId);
		}
		else if(action == MP_PlayerAction::BUILD)
		{
			emit build(playerId, fieldId);
		}
		else if(action == MP_PlayerAction::DESTRUCT)
		{
			emit destruct(playerId, fieldId);
		}

		return;
	}

	if(type == PacketType::REQUEST_PAYMENT)
	{
		RequestPaymentPacket packet(data);

		int playerId = packet.getPlayerId();
		float amount = packet.getAmount();

		emit paymentFailed(playerId, amount);

		return;
	}

	if(type == PacketType::UPDATE_DASHBOARD)
	{
		UpdateDashboardPacket packet(data);

		QVector<PlayerAction> actions = packet.getActions();

		emit updateDashboard(actions);

		return;
	}

	if(type == PacketType::UPDATE_PING)
	{
		UpdatePingPacket packet(data);

		int playerId = packet.getPlayerId();
		int ping     = packet.getPing();

		emit updatePing(playerId, ping);

		return;
	}

	if(type == PacketType::TRADE)
	{
		TradePacket packet(data);

		if(packet.getStatus() == TradeStatus::PENDING)
		{
			emit tradeOffered(packet);
		}
		else if(packet.getStatus() == TradeStatus::DENIED)
		{
			emit tradeDenied(packet);
		}
		else
		{
			emit tradeAccepted(packet);
		}

		return;
	}

	if(type == PacketType::NOTIFICATION)
	{
		NotificationPacket packet(data);

		QString description = packet.getDescription();

		NotificationType type = packet.getType();

		if(type == NotificationType::ERROR)
		{
			NotificationManager::pushNotification("Napaka",
												description,
												 NotificationType::ERROR);
		}
		else if(type == NotificationType::WARNING)
		{
			NotificationManager::pushNotification("Opozorilo",
												description,
												 NotificationType::WARNING);
		}
		else
		{
			NotificationManager::pushNotification("Uspeh",
												  description,
													NotificationType::INFORMATION);
		}

		return;
	}
}

void LocalClient::sendPingPacket()
{
	pingSendTimer->stop();

	pingSentTime = QTime::currentTime();

	PingPacket packet(Sender::CLIENT);

	this->getSocket()->write(packet.getData(true));
}
