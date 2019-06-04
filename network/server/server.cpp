#include "server.h"

#include <QString>
#include <QMessageBox>
#include <QNetworkInterface>

#include "core/game/board/board.h"
#include "core/game/entity/dice.h"
#include "core/game/entity/token.h"
#include "core/game/managers/fieldsmanager.h"
#include "core/game/managers/cardsmanager.h"
#include "core/game/entity/fields/possessablefield.h"

#include "core/ui/extensions/components/sidebar/participantsview.h"

#include "network/packets/movepacket.h"
#include "network/packets/stoppacket.h"
#include "network/packets/chatpacket.h"
#include "network/packets/startpacket.h"
#include "network/packets/errorpacket.h"
#include "network/packets/setturnpacket.h"
#include "network/packets/depositpacket.h"
#include "network/packets/connectpacket.h"
#include "network/packets/withdrawpacket.h"
#include "network/packets/disconnectpacket.h"
#include "network/packets/updatepingpacket.h"
#include "network/packets/serverdownpacket.h"
#include "network/packets/updateboardpacket.h"
#include "network/packets/notificationpacket.h"

#include "core/utils/errorUtils/errorhandler.h"

#include "core/utils/playerUtils/playerutils.h"

#include "network/server/serverchat.h"

#include "network/server/serverplayeractionmanager.h"

Server::Server(QObject *parent)
	: QObject(parent)
{
	// Find the Local IP that is not 127.0.0.1
	for(QHostAddress addr : QNetworkInterface::allAddresses())
	{
		if((addr != QHostAddress::LocalHost)
				&& (addr.protocol() == QAbstractSocket::IPv4Protocol))
		{
			address = addr;

			break;
		}
	}

	gameActive = false;

	actionManager = nullptr;

	tcpServer = new QTcpServer(parent);

	manager.getBoard()->setFields(
				FieldsManager::getDefaultFields(nullptr));
	manager.getBoard()->setCards(CardsManager::getDefaultCards());

	// Do not show the server-side cards
	for(Card* card : manager.getBoard()->getCards())
	{
		card->setInvisible(true);
	}

	this->startListening();

	connect(tcpServer, SIGNAL(newConnection()),
				this, SLOT(handleNewConnection()));

	// Delete this server when the parent is closed
	connect(parent, SIGNAL(destroyed(QObject*)),
				this, SLOT(deleteLater()));

	chat = new ServerChat(this, &manager);
}

Server::~Server()
{
	ServerDownPacket packet(QString("Strežnik je bil ugasnjen!"));

	for(ServerClient *client : clients)
	{
		client->getSocket()->write(packet.getData(true));

		// Wait for the message to be sent before proceeding
		// This ensures that the packets are sent to the clients
		// Before the server is closed (destructor finished)
		if(client->getSocket()->waitForBytesWritten()) {}
	}
}

bool Server::clientExists(QTcpSocket *socket)
{
	QHostAddress address = socket->peerAddress();
	qint16 port = socket->peerPort();

	for(ServerClient *client : clients)
	{
		if((client->getHostAddress() == address)
				&& (client->getPort() == port))
		{
			return true;
		}
	}

	return false;
}

QHostAddress Server::getAddress()
{
	return address;
}

ServerChat* Server::getChat()
{
	return this->chat;
}

BoardManager* Server::getBoardManager()
{
	return &manager;
}

void Server::setParticipantsView(ParticipantsView *view)
{
	this->participants = view;

	connect(this->manager.getBoard(), SIGNAL(playerTurnChanged()),
				this, SLOT(onTurnChanged()));
}

bool Server::startGame()
{
	if(!gameActive)
	{
		Player* startingPlayer = this->getStartingPlayer();
		QString playerName = startingPlayer->getName();

		UpdateBoardPacket updateBoardPacket(manager.getBoard());
		StartPacket startPacket;

		for(Client* client : this->clients)
		{
			client->getSocket()->write(updateBoardPacket.getData(true));
			client->getSocket()->write(startPacket.getData(true));
		}

		actionManager = new ServerPlayerActionManager(this->clients, &manager,
														this);

		gameActive = true;

		this->chat->addText(playerName + " je vrgel najvišjo vrednost kock"
								+ ", zato gre prvi.", true);

		return true;
	}

	return false;
}

Player* Server::getStartingPlayer()
{
	int highestDiceValue = 0;
	int playerId = -1;

	Dice* dice = manager.getBoard()->getDice();

	for(Player* player : manager.getBoard()->getPlayers())
	{
		dice->roll();

		if((dice->getFirst() + dice->getSecond()) > highestDiceValue)
		{
			highestDiceValue = dice->getFirst() + dice->getSecond();

			playerId = player->getId();
		}
	}

	manager.getBoard()->setPlayerTurn(playerId);

	Player* startingPlayer = manager.getPlayer(playerId);

	return startingPlayer;
}

bool Server::stopGame()
{
	if(gameActive)
	{
		StopPacket stopPacket;

		gameActive = false;

		for(Client* client : this->clients)
		{
			client->getSocket()->write(stopPacket.getData(true));
		}

		return true;
	}

	return false;
}

void Server::deposit(const float &amount, const int &playerId)
{
	DepositPacket depositPacket(playerId, amount);

	this->manager.getPlayer(playerId)->deposit(amount);

	for(Client* client : this->clients)
	{
		client->getSocket()->write(depositPacket.getData(true));
	}
}

bool Server::withdraw(const float &amount, const int &playerId)
{
	if(this->manager.getPlayer(playerId)->withdraw(amount))
	{
		WithdrawPacket withdrawPacket(playerId, amount);

		for(Client* client : this->clients)
		{
			client->getSocket()->write(withdrawPacket.getData(true));
		}

		return true;
	}

	return false;
}

void Server::move(const int &targetFieldId, const int &playerId)
{
	MovePacket movePacket(playerId, targetFieldId);

	Field* field = this->manager.getField(targetFieldId);

	Player* player = this->manager.getPlayer(playerId);

	player->getToken()->move(field);

	for(Client* client : this->clients)
	{
		client->getSocket()->write(movePacket.getData(true));
	}
}

void Server::startListening()
{
	if(!tcpServer->listen(address, SERVER_PORT))
	{
		ErrorHandler::log("Server::startListening",
							tcpServer->errorString());
	}
}

void Server::handleNewConnection()
{
	while(tcpServer->hasPendingConnections())
	{
		QTcpSocket *socket = tcpServer->nextPendingConnection();
		if(!clientExists(socket))
		{
			ServerClient *client = new ServerClient(socket, this);
			clients.append(client);

			if(manager.getBoard()->getPlayers().size() == 6)
			{
				ErrorPacket packet(ErrorCode::GAME_FULL,
									"Igra je že polna.");

				client->getSocket()->write(packet.getData(true));

				return;
			}

			connect(client, SIGNAL(playerConnected(NetworkPlayer*)),
						this, SLOT(addPlayer(NetworkPlayer*)));

			connect(client, SIGNAL(playerDisconnected(int)),
						this, SLOT(removePlayer(int)));

			connect(client, SIGNAL(messageReceived(Message)),
						this, SLOT(handleMessageReceived(Message)));

			// Delete clients when the server is deleted
			client->connect(this, SIGNAL(destroyed(QObject*)),
								client, SLOT(deleteLater()));

			if(gameActive)
			{
				ErrorPacket errorPacket(ErrorCode::GAME_ACTIVE, "Igra že teče!");

				client->getSocket()->write(errorPacket.getData(true));
			}
			else
			{
				// Send players already connected to the newly
				//     connected client
				for(Player* p : manager.getBoard()->getPlayers())
				{
					NetworkPlayer *player = (NetworkPlayer*)p;

					ConnectPacket packet(player);

					client->getSocket()->write(packet.getData(true));
				}
			}
		}
	}
}

void Server::addPlayer(NetworkPlayer *player)
{
	ServerClient* sender = qobject_cast<ServerClient*> (QObject::sender());

	if(manager.getBoard()->getPlayers().size() == 6)
	{
		ErrorPacket packet(ErrorCode::GAME_FULL,
							"Igra je že polna.");

		sender->getSocket()->write(packet.getData(true));

		return;
	}

	QVector<Player*> players = manager.getBoard()->getPlayers();

	// Check if a player with the same name already exists
	for(Player *p : players)
	{
		if(p->getName() == player->getName())
		{
			ErrorPacket packet(ErrorCode::PLAYER_EXISTS,
								"Igralec z imenom " + player->getName()
									 + " že obstaja.");

			sender->getSocket()->write(packet.getData(true));

			return;
		}
	}

	if(availableIdList.size() > 0)
	{
		int nextId = availableIdList[0];

		player = new NetworkPlayer(player->getToken(), player->getName(),
									nextId);

		availableIdList.remove(0);
	}
	else
	{
		player = new NetworkPlayer(player->getToken(), player->getName());
	}

	chat->addClient(sender);

	players.append(player);

	manager.getBoard()->setPlayers(players);

	sender->setPlayer(player);

	connect(sender, SIGNAL(updatePing(int,int)),
				participants, SLOT(refreshPing(int,int)));

	connect(sender, SIGNAL(updatePing(int,int)),
				this, SLOT(sendPingUpdate(int,int)));

	connect(sender, SIGNAL(tradePacketReceived(TradePacket)),
				this, SLOT(onTradePacketReceived(TradePacket)));

	emit playerAdded(player);

	ConnectPacket joinPacket(player);

	for(ServerClient *client : this->clients)
	{
		client->getSocket()->write(joinPacket.getData(true));
	}

	this->chat->addText(player->getName() + " se je pridružil igri.", true);

	NotificationPacket notificationPacket("Uspešno ste se pridružili igri.",
											NotificationType::INFORMATION);

	sender->getSocket()->write(notificationPacket.getData(true));
}

void Server::removePlayer(int playerId)
{
	QVector<Player*> players = manager.getBoard()->getPlayers();

	QString playerName;

	for(Player* player : players)
	{
		if(player->getId() == playerId)
		{
			this->actionManager->transferFunds(player);

			playerName = player->getName();

			players.removeAll(player);

			if(players.size() == 0)
			{
				PlayerUtils::resetNetworkPlayerId();
			}

			manager.getBoard()->setPlayers(players);

			availableIdList.append(playerId);

			emit playerRemoved(player);

			break;
		}
	}

	DisconnectPacket disconnectPacket(playerId);

	for(ServerClient* client : this->clients)
	{
		client->getSocket()->write(disconnectPacket.getData(true));
	}

	this->chat->addText(playerName + " je zapustil igro.", true);

	if(gameActive)
	{
		if((manager.getBoard()->getPlayerTurn() == playerId)
				&& (players.count() > 1))
		{
			manager.setNextPlayerTurn();

			SetTurnPacket setTurnPacket(manager.getBoard()->getPlayerTurn());

			for(ServerClient* client : this->clients)
			{
				client->getSocket()->write(setTurnPacket.getData(true));
			}
		}
		else if(players.count() == 1)
		{
			Player* winner = players[0];

			QString winnerName = winner->getName();

			NotificationPacket packet("Zmagovalec je " + winnerName,
										NotificationType::INFORMATION);

			for(ServerClient* client : this->clients)
			{
				client->getSocket()->write(packet.getData(true));
			}

			stopGame();
		}
		else if(players.count() < 1)
		{
			stopGame();
		}
	}
}

void Server::handleMessageReceived(Message msg)
{
	int senderId = msg.getSenderId();
	QString text = msg.getText();

	ChatPacket chatPacket(senderId, text);

	for(ServerClient* client : this->clients)
	{
		client->getSocket()->write(chatPacket.getData(true));
	}
}

void Server::sendPingUpdate(int playerId, int ping)
{
	UpdatePingPacket packet(playerId, ping);

	for(Client* client : this->clients)
	{
		client->getSocket()->write(packet.getData(true));
	}
}

void Server::onTradePacketReceived(TradePacket packet)
{
	TradeStatus status = packet.getStatus();

	if(status == TradeStatus::ACCEPTED)
	{
		QVector<Field*> fields = manager.getBoard()->getFields();

		int providerId  = packet.getProviderId();
		int recipientId = packet.getRecipientId();

		Player* provider  = manager.getPlayer(providerId);
		Player* recipient = manager.getPlayer(recipientId);

		OfferPair provided = packet.getProvided(fields);
		OfferPair returned = packet.getReturned(fields);

		Offer* offer = new Offer(provider);

		offer->setRecipient(recipient);
		offer->setOfferPairProvided(provided);
		offer->setOfferPairReturned(returned);

		offer->acceptOffer();
	}

	for(Client* client : this->clients)
	{
		client->getSocket()->write(packet.getData(true));
	}

	actionManager->onOfferAccepted();
}

void Server::onTurnChanged()
{
	int playerTurn = manager.getBoard()->getPlayerTurn();

	Player* player = manager.getPlayer(playerTurn);

	participants->setPlayerTurn(player);
}
