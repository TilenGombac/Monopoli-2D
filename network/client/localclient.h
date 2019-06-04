#ifndef LOCALCLIENT_H
#define LOCALCLIENT_H

#include "network/client/client.h"
#include "network/packets/tradepacket.h"
#include "core/game/player/networkplayer.h"
#include "network/packets/updateboardpacket.h"
#include "network/packets/updatedashboardpacket.h"

class LocalClient : public Client
{
	Q_OBJECT

public:
	LocalClient(QHostAddress serverAddress, QObject *parent = nullptr);

public slots:
	void processPacket(QByteArray data);

private slots:
	void sendPingPacket();

signals:
	void connectionStatus(bool ok);
	void playerExists();
	void gameAlreadyInProgress();
	void gameFull();
	void playerConnected(NetworkPlayer *player);
	void gameStarted();
	void gameStopped();
	void fieldBought(int playerId);
	void serverOffline(QString reason);
	void deposit(int playerId, float amount);
	void withdraw(int playerId, float amount);
	void move(int playerId, int fieldId);
	void boardUpdated(UpdateBoardPacket packet);
	void diceUpdated(int first, int second);
	void sendToJail(int playerId);
	void leaveJail(int playerId);
	void turnChanged(int playerId);
	void mortgage(int playerId, int fieldId);
	void unmortgage(int playerId, int fieldId);
	void build(int playerId, int fieldId);
	void destruct(int playerId, int fieldId);
	void paymentFailed(int playerId, float amount);
	void updateDashboard(QVector<PlayerAction> actions);
	void updatePing(int playerId, int ping);

	void tradeOffered(TradePacket packet);
	void tradeDenied(TradePacket packet);
	void tradeAccepted(TradePacket packet);

};

#endif // LOCALCLIENT_H
