#include "mpgame.h"

#include <QApplication>
#include <QSystemTrayIcon>

#include "core/game/board/board.h"
#include "core/game/entity/dice.h"
#include "core/game/entity/token.h"
#include "core/game/player/player.h"
#include "core/ui/extensions/sidebar.h"
#include "core/ui/extensions/mainview.h"
#include "core/utils/gameUtils/gameutils.h"
#include "core/game/managers/boardmanager.h"
#include "core/utils/errorUtils/errorhandler.h"
#include "core/game/managers/fieldvisitmanager.h"
#include "core/game/entity/fields/possessablefield.h"
#include "core/ui/extensions/components/mainView/home.h"
#include "core/ui/extensions/components/sidebar/mpdashboard.h"
#include "core/ui/extensions/components/sidebar/fielddisplay.h"
#include "core/ui/extensions/components/sidebar/participantsview.h"
#include "core/ui/extensions/components/mainView/boardDisplay/dicedisplay.h"
#include "core/ui/extensions/components/mainView/boardDisplay/boarddisplay.h"

#include "network/client/localclient.h"

MPGame::MPGame(LocalClient* client, MainView *mainView,
				BoardDisplay *setBoardDisplay,
					ParticipantsView* participantsView, Player* localPlayer)
	: QObject()
{
	this->client = client;

	this->localPlayer = localPlayer;

	boardDisplay = setBoardDisplay;
	home         = mainView->getHome();
	sidebar      = mainView->getSidebar();
	boardManager = boardDisplay->getBoardManager();

	this->trayIcon = new QSystemTrayIcon(QIcon(":/img/img/icon.png"), this);
	this->trayIcon->setToolTip("Monopoli 2D večigralski način");
	this->trayIcon->show();

	this->participantsView = participantsView;

	Dice *dice = boardManager->getBoard()->getDice();

	QVector<Field*> fields = boardManager->getBoard()->getFields();

	diceDisplay  = new DiceDisplay(dice, boardDisplay);
	fieldDisplay = new FieldDisplay(fields, sidebar);

	dashboard = new MPDashboard(boardManager, localPlayer, fieldDisplay, client,
									sidebar);

	connect(boardManager->getBoard(), SIGNAL(playerTurnChanged()),
				dashboard, SLOT(updateDashboard()));

	visitManager = new FieldVisitManager(boardManager);

	boardDisplay->show();

	for(Field *field : fields)
	{
		if(field->getPossessable())
		{
			PossessableField *tmp = dynamic_cast<PossessableField*> (field);
			connect(tmp, SIGNAL(changeEvent(QEvent*)),
						boardDisplay, SLOT(repaint()));
		}
	}

	connect(client, SIGNAL(move(int,int)),
				this, SLOT(onMoveReceived(int,int)));

	connect(client, SIGNAL(deposit(int,float)),
				this, SLOT(onDepositReceived(int,float)));

	connect(client, SIGNAL(withdraw(int,float)),
				this, SLOT(onWithdrawReceived(int,float)));

	connect(client, SIGNAL(diceUpdated(int,int)),
				this, SLOT(onDiceUpdatedReceived(int,int)));

	connect(client, SIGNAL(sendToJail(int)),
				this, SLOT(onSendToJailReceived(int)));

	connect(client, SIGNAL(leaveJail(int)),
				this, SLOT(onLeaveJailReceived(int)));

	connect(client, SIGNAL(fieldBought(int)),
				this, SLOT(onFieldBoughtReceived(int)));

	connect(client, SIGNAL(turnChanged(int)),
				this, SLOT(onTurnChangedReceived(int)));

	connect(client, SIGNAL(unmortgage(int,int)),
				this, SLOT(onUnmortgageReceived(int,int)));

	connect(client, SIGNAL(mortgage(int,int)),
				this, SLOT(onMortgageReceived(int,int)));

	connect(client, SIGNAL(build(int,int)),
				this, SLOT(onBuildReceived(int,int)));

	connect(client, SIGNAL(destruct(int,int)),
				this, SLOT(onDestructReceived(int,int)));

	connect(client, SIGNAL(serverOffline(QString)),
				this, SLOT(onServerDown(QString)));

	connect(client, SIGNAL(gameStopped()),
				this, SLOT(onGameOver()));
}

MPGame::~MPGame()
{
	fieldDisplay->deleteLater();
	dashboard->deleteLater();
}

void MPGame::onMoveReceived(int playerId, int fieldId)
{
	Player* player = boardManager->getPlayer(playerId);
	Field* field   = boardManager->getField(fieldId);

	player->getToken()->move(field);
}

void MPGame::onDepositReceived(int playerId, float amount)
{
	Player* player = boardManager->getPlayer(playerId);

	player->deposit(amount);
}

void MPGame::onWithdrawReceived(int playerId, float amount)
{
	Player* player = boardManager->getPlayer(playerId);

	player->withdraw(amount);
}

void MPGame::onDiceUpdatedReceived(int first, int second)
{
	boardManager->getBoard()->getDice()->setValue(first, second);
}

void MPGame::onSendToJailReceived(int playerId)
{
	Player* player = boardManager->getPlayer(playerId);

	if(player != nullptr)
	{
		boardManager->sendToJail(player);
	}
}

void MPGame::onLeaveJailReceived(int playerId)
{
	Player* player = boardManager->getPlayer(playerId);

	if(player != nullptr)
	{
		player->leaveJail();
	}
}

void MPGame::onFieldBoughtReceived(int playerId)
{
	Player* player = boardManager->getPlayer(playerId);

	if(player != nullptr)
	{
		Field* field = player->getToken()->getField();

		player->purchase(field);
	}
}

void MPGame::onTurnChangedReceived(int playerId)
{
	if(playerId != -1)
	{
		boardManager->getBoard()->setPlayerTurn(playerId);

		Player* player = boardManager->getPlayer(playerId);

		participantsView->setPlayerTurn(player);

		if(player == this->localPlayer)
		{
			// No window in this app has focus, display system
			// notification of player's turn
			if(QApplication::focusWidget() == 0)
			{
				this->trayIcon->showMessage("Opozorilo", "Vi ste na potezi.");
			}
		}
	}
}

void MPGame::onMortgageReceived(int playerId, int fieldId)
{
	Player* player = boardManager->getPlayer(playerId);

	if(player != nullptr)
	{
		Field* field = boardManager->getField(fieldId);

		player->mortgage(field);
	}
}

void MPGame::onUnmortgageReceived(int playerId, int fieldId)
{
	Player* player = boardManager->getPlayer(playerId);

	if(player != nullptr)
	{
		Field* field = boardManager->getField(fieldId);

		player->unmortgage(field);
	}
}

void MPGame::onBuildReceived(int playerId, int fieldId)
{
	Player* player = boardManager->getPlayer(playerId);

	if(player != nullptr)
	{
		Field* field = boardManager->getField(fieldId);

		GameUtils::addBuilding(field);

		Action action = field->getAction();

		action.setNoAction();

		field->setAction(action);
	}
}

void MPGame::onDestructReceived(int playerId, int fieldId)
{
	Player* player = boardManager->getPlayer(playerId);

	if(player != nullptr)
	{
		Field* field = boardManager->getField(fieldId);

		GameUtils::removeBuilding(field);

		Action action = field->getAction();

		action.setNoAction();

		field->setAction(action);
	}
}

void MPGame::onServerDown(QString reason)
{
	Q_UNUSED(reason)

	// The server down handling is already handled in the networklobby

	this->deleteLater();
}

void MPGame::onGameOver()
{
	this->deleteLater();
}
