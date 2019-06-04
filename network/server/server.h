#ifndef SERVER_H
#define SERVER_H

#include <QVector>
#include <QTcpServer>

#include "network/packets/packet.h"
#include "network/client/serverclient.h"
#include "core/game/managers/boardmanager.h"

class ServerChat;
class ParticipantsView;
class ServerPlayerActionManager;

class Server : public QObject
{
	Q_OBJECT

public:
	Server(QObject *parent = nullptr);
	~Server();

	bool clientExists(QTcpSocket *socket);

	QHostAddress getAddress();

	ServerChat* getChat();

	BoardManager* getBoardManager();
	ServerPlayerActionManager* actionManager;

	void setParticipantsView(ParticipantsView* view);

	bool startGame();
	Player* getStartingPlayer();
	bool stopGame();
	void deposit(const float &amount, const int &playerId);
	bool withdraw(const float &amount, const int &playerId);
	void move(const int &targetFieldId, const int &playerId);

public slots:
	void startListening();
	void handleNewConnection();
	void addPlayer(NetworkPlayer *player);
	void removePlayer(int playerId);
	void handleMessageReceived(Message msg);
	void sendPingUpdate(int playerId, int ping);
	void onTradePacketReceived(TradePacket packet);

private slots:
	void onTurnChanged();

private:
	bool gameActive;
	QHostAddress address;
	QTcpServer *tcpServer;
	BoardManager manager;
	QVector<ServerClient*> clients;
	QVector<int> availableIdList;
	ServerChat* chat;

	ParticipantsView* participants;

signals:
	void playerAdded(Player* player);
	void playerRemoved(Player* player);

};

#endif // SERVER_H
