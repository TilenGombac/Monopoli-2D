#ifndef PLAYERDASHBOARD_H
#define PLAYERDASHBOARD_H

#include <QWidget>
#include <QToolButton>

#include "core/game/managers/playeractionmanager.h"

class FieldDisplay;

enum class PlayerAction
{
	UNMORTGAGE = 0,
	ROLL_DICE  = 1,
	MORTGAGE   = 2,
	END_TURN   = 3,
	DESTRUCT   = 4,
	BUILD      = 5,
	TRADE      = 6,
	BUY        = 7,
	LEAVE_JAIL = 8
};

class PlayerDashboard : public QWidget
{
	Q_OBJECT

	Player *player;
	BoardManager *boardManager;
	PlayerActionManager *actionManager;

	FieldDisplay *fieldDisplay;

	// 9 actions
	QToolButton *buttons[9];

	bool paymentOnHold;
	float debt;

public:
	PlayerDashboard(BoardManager *manager, Player *setPlayer,
						FieldDisplay *setFieldDisplay,
							QWidget *sidebar = 0);

private:
	void initializeButtons();

public slots:
	void updateDashboard();
	void handlePaymentFailed(float amount);
	void handlePaymentSuccess();
	void updateSelectedField();
	void handleAcceptedOffer(Player *recipient);
	void handleDeniedOffer(Player *recipient);
	void goBankrupt();
	void attemptPayment();
	void leaveJail();

};

#endif // PLAYERDASHBOARD_H
