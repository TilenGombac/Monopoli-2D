#include "serverclient.h"

#include "network/packets/buypacket.h"
#include "network/packets/jailpacket.h"
#include "network/packets/pingpacket.h"
#include "network/packets/chatpacket.h"
#include "network/packets/errorpacket.h"
#include "network/packets/connectpacket.h"
#include "network/packets/endturnpacket.h"
#include "network/packets/rolldicepacket.h"
#include "network/packets/disconnectpacket.h"
#include "network/packets/playeractionpacket.h"

#include "core/utils/errorUtils/errorhandler.h"

ServerClient::ServerClient(QTcpSocket *socket, QObject *parent)
	: Client(parent)
{
	this->socket = socket;

	player = nullptr;

	connect(socket, SIGNAL(readyRead()), this, SLOT(preparePacket()));
}

void ServerClient::setPlayer(NetworkPlayer *player)
{
	this->player = player;

	connect(this, SIGNAL(pingChanged(int)),
				this, SLOT(sendPingUpdate(int)));

	this->sendPingUpdate(this->ping);
}

NetworkPlayer* ServerClient::getPlayer()
{
	return player;
}

void ServerClient::processPacket(QByteArray data)
{
	PacketType type = Packet::determineType(data);

	switch(type)
	{
		case PacketType::CONNECT:
		{
			ConnectPacket packet(data);

			NetworkPlayer *player = packet.getPlayer();

			if(player != nullptr)
			{
				emit playerConnected(player);
			}
			else
			{
				ErrorHandler::log("Nullptr player!");
			}

			break;
		}
		case PacketType::DISCONNECT:
		{
			DisconnectPacket packet(data);

			int playerId = packet.getPlayerId();

			emit playerDisconnected(playerId);

			break;
		}
		case PacketType::PING:
		{
			PingPacket packet(data);

			Sender sender = packet.getSender();

			if(sender == Sender::SERVER)
			{
				this->calculatePing();
			}
			else if(sender == Sender::CLIENT)
			{
				this->getSocket()->write(packet.getData(true));
			}

			break;

		}
		case PacketType::ERROR:
		{
			ErrorPacket packet(data);

			QString errorMessage = packet.getErrorMessage();

			ErrorHandler::message("Prišlo je do napake", errorMessage);

			break;
		}

		case PacketType::CHAT:
		{
			ChatPacket packet(data);

			int senderId = packet.getSenderId();
			QString text = packet.getMessage();

			Message msg(senderId, text);

			emit messageReceived(msg);

			break;
		}

		case PacketType::ROLL_DICE:
		{
			RollDicePacket packet(data);

			int playerId = packet.getPlayerId();

			emit diceRolled(playerId);

			break;
		}

		case PacketType::END_TURN:
		{
			EndTurnPacket packet(data);

			int playerId = packet.getPlayerId();

			emit endTurn(playerId);

			break;
		}

		case PacketType::BUY:
		{
			BuyPacket packet(data);

			int playerId = packet.getPlayerId();

			emit buyRequest(playerId);

			break;
		}

		case PacketType::PLAYER_ACTION:
		{
			PlayerActionPacket packet(data);

			int playerId = packet.getPlayerId();
			int fieldId  = packet.getFieldId();

			MP_PlayerAction action = packet.getPlayerAction();

			if(action == MP_PlayerAction::UNMORTGAGE)
			{
				emit unmortgageRequest(playerId, fieldId);
			}
			else if(action == MP_PlayerAction::MORTGAGE)
			{
				emit mortgageRequest(playerId, fieldId);
			}
			if(action == MP_PlayerAction::BUILD)
			{
				emit buildRequest(playerId, fieldId);
			}
			else if(action == MP_PlayerAction::DESTRUCT)
			{
				emit destructRequest(playerId, fieldId);
			}

			break;
		}

		case PacketType::TRADE:
		{
			TradePacket packet(data);

			emit tradePacketReceived(packet);

			break;
		}

		case PacketType::JAIL:
		{
			JailPacket packet(data);

			int playerId = packet.getPlayerId();

			JailAction action = packet.getAction();

			if(action == JailAction::LEAVE_JAIL)
			{
				emit leaveJailRequest(playerId);
			}

			break;
		}

		default:
		{
			ErrorHandler::log("Invalid or unhandled packet");

			break;
		}
	}
}

void ServerClient::sendPingUpdate(int currentPing)
{
	if(player != nullptr)
	{
		int playerId = player->getId();

		emit updatePing(playerId, currentPing);
	}
}

void ServerClient::sendPingPacket()
{
	pingSendTimer->stop();

	pingSentTime = QTime::currentTime();

	PingPacket packet(Sender::SERVER);

	this->getSocket()->write(packet.getData(true));
}
