#ifndef MPDASHBOARD_H
#define MPDASHBOARD_H

#include <QWidget>
#include <QToolButton>

#include "network/packets/tradepacket.h"

class Player;
class LocalClient;
class FieldDisplay;
class BoardManager;
class MPActionManager;

enum class PlayerAction;

class MPDashboard : public QWidget
{
	Q_OBJECT

	Player* player;
	LocalClient* client;
	FieldDisplay* fieldDisplay;
	BoardManager* boardManager;
	MPActionManager* actionManager;

	bool rollDiceAvailable;
	bool endTurnAvailable;

	// 9 actions
	QToolButton* buttons[9];

public:
	MPDashboard(BoardManager* bm, Player* localPlayer,
					FieldDisplay* fieldDisplay, LocalClient* client,
						QWidget* sidebar = 0);

	void initializeButtons();

public slots:
	void updateDashboard();
	void onUpdateDashboardReceived(QVector<PlayerAction> actions);

	void onPaymentFailedReceived(int playerId, float amount);

	void rollDice();
	void endTurn();
	void buy();
	void trade();
	void mortgage();
	void unmortgage();
	void build();
	void destruct();
	void leaveJail();
	void startTradeSetup();

	void onTradeOffered(TradePacket packet);
	void onTradeDenied(TradePacket packet);
	void onTradeAccepted(TradePacket packet);

	void denyOffer(TradePacket packet);
	void acceptOffer(TradePacket packet);

	void sendOffer(Offer* offer);

private slots:
	void stopTrade();

};

#endif // MPDASHBOARD_H
