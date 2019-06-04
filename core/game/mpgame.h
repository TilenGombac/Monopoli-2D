#ifndef MPGAME_H
#define MPGAME_H

#include <QObject>

class Home;
class Player;
class Sidebar;
class MainView;
class DiceDisplay;
class LocalClient;
class MPDashboard;
class BoardDisplay;
class BoardManager;
class FieldDisplay;
class QSystemTrayIcon;
class ParticipantsView;
class FieldVisitManager;

class MPGame : public QObject
{
	Q_OBJECT

	Home* home;
	Sidebar* sidebar;

	QSystemTrayIcon* trayIcon;

	BoardDisplay* boardDisplay;
	BoardManager* boardManager;

	DiceDisplay* diceDisplay;
	FieldDisplay* fieldDisplay;

	LocalClient* client;

	MPDashboard* dashboard;

	ParticipantsView* participantsView;

	FieldVisitManager* visitManager;

	Player* localPlayer;

public:
	MPGame(LocalClient* client, MainView* mainView,
			BoardDisplay* setBoardDisplay,
				ParticipantsView* participantsView,
					Player* localPlayer);

	~MPGame();

public slots:
	void onMoveReceived(int playerId, int fieldId);
	void onDepositReceived(int playerId, float amount);
	void onWithdrawReceived(int playerId, float amount);
	void onDiceUpdatedReceived(int first, int second);
	void onSendToJailReceived(int playerId);
	void onLeaveJailReceived(int playerId);
	void onFieldBoughtReceived(int playerId);
	void onTurnChangedReceived(int playerId);
	void onMortgageReceived(int playerId, int fieldId);
	void onUnmortgageReceived(int playerId, int fieldId);
	void onBuildReceived(int playerId, int fieldId);
	void onDestructReceived(int playerId, int fieldId);
	void onServerDown(QString reason);
	void onGameOver();

};

#endif // MPGAME_H
