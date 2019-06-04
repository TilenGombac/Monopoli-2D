#ifndef SERVERPLAYERACTIONMANAGER_H
#define SERVERPLAYERACTIONMANAGER_H

#include <QObject>

#include "network/client/serverclient.h"

class BoardManager;
class FieldVisitManager;

class ServerPlayerActionManager : public QObject
{
	Q_OBJECT

	QVector<ServerClient*> serverClients;
	BoardManager* boardManager;

	FieldVisitManager* visitManager;

	bool pendingPayment;
	float debt;

	bool diceRolled;

public:
	ServerPlayerActionManager(QVector<ServerClient*> clients,
								BoardManager* bm, QObject* parent = 0);
	~ServerPlayerActionManager();

private:
	QTcpSocket* getPlayerSocket(int playerId);

public slots:
	void sendAvailableActions();
	void onDiceRolledReceived(int playerId);
	void onEndTurnReceived(int playerId);
	void onBuyReceived(int playerId);
	void onUnmortgageReceived(int playerId, int fieldId);
	void onMortgageReceived(int playerId, int fieldId);
	void onBuildReceived(int playerId, int fieldId);
	void onDestructReceived(int playerId, int fieldId);
	void onLeaveJailReceived(int playerId);
	void onPaymentFailedReceived(float amount);
	void onPaymentSuccessReceived(float amount);

	void onOfferAccepted();

	void transferFunds(Player* player);

};

#endif // SERVERPLAYERACTIONMANAGER_H
