#ifndef SERVERCLIENT_H
#define SERVERCLIENT_H

#include "network/client/client.h"
#include "network/packets/tradepacket.h"
#include "core/game/player/networkplayer.h"

class ServerClient : public Client
{
	Q_OBJECT

public:
	ServerClient(QTcpSocket *socket, QObject *parent = nullptr);

	// The playerConnected is not the actual player pointer pushed
	// to the player list
	void setPlayer(NetworkPlayer* player);

	NetworkPlayer* getPlayer();

public slots:
	void processPacket(QByteArray data);

	void sendPingUpdate(int currentPing);

private slots:
	void sendPingPacket();

signals:
	void playerConnected(NetworkPlayer *player);
	void diceRolled(int playerId);
	void endTurn(int playerId);
	void buyRequest(int playerId);
	void mortgageRequest(int playerId, int fieldId);
	void unmortgageRequest(int playerId, int fieldId);
	void buildRequest(int playerId, int fieldId);
	void destructRequest(int playerId, int fieldId);
	void leaveJailRequest(int playerId);
	void updatePing(int playerId, int ping);

	void tradePacketReceived(TradePacket packet);

private:
	NetworkPlayer *player;

};

#endif // SERVERCLIENT_H
