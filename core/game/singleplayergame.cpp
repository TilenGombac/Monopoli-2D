#include "singleplayergame.h"

#include "core/game/player/ai.h"
#include "core/game/board/board.h"
#include "core/game/entity/dice.h"
#include "core/game/entity/field.h"
#include "core/game/entity/token.h"
#include "core/game/player/player.h"
#include "core/ui/extensions/sidebar.h"
#include "core/ui/extensions/mainview.h"
#include "core/game/managers/boardmanager.h"
#include "core/utils/errorUtils/errorhandler.h"
#include "core/game/managers/fieldvisitmanager.h"
#include "core/notifications/notificationmanager.h"
#include "core/game/entity/fields/possessablefield.h"
#include "core/ui/extensions/components/sidebar/fielddisplay.h"
#include "core/ui/extensions/components/sidebar/playerdashboard.h"
#include "core/ui/extensions/components/sidebar/participantsview.h"
#include "core/ui/extensions/components/mainView/boardDisplay/dicedisplay.h"
#include "core/ui/extensions/components/mainView/boardDisplay/boarddisplay.h"

SinglePlayerGame::SinglePlayerGame(MainView *mainView,
									BoardDisplay *setBoardDisplay)
	: QObject()
{
	boardDisplay = setBoardDisplay;
	sidebar      = mainView->getSidebar();
	boardManager = boardDisplay->getBoardManager();

	Dice *dice = boardManager->getBoard()->getDice();

	QVector<Field*> fields = boardManager->getBoard()->getFields();

	diceDisplay  = new DiceDisplay(dice, boardDisplay);
	fieldDisplay = new FieldDisplay(fields, sidebar);

	view = new ParticipantsView(boardManager->getBoard()->getPlayers(),
									sidebar);

	connect(boardManager, SIGNAL(playerRemoved(Player*)),
				view, SLOT(removeParticipant(Player*)));

	new FieldVisitManager(boardManager);

	boardDisplay->show();

	setStartingPlayer();

	connect(boardManager->getBoard(), SIGNAL(playerTurnChanged()),
				this, SLOT(callAI()));

	connect(boardManager->getBoard(), SIGNAL(playerTurnChanged()),
				this, SLOT(updateParticipantsTurn()));

	for(Player *player : boardManager->getBoard()->getPlayers())
	{
		if(player->getPlayerType() == PlayerType::LOCAL)
		{
			playerDashboard = new PlayerDashboard(boardManager, player,
													fieldDisplay, sidebar);

			connect(boardManager->getBoard(), SIGNAL(playerTurnChanged()),
						playerDashboard, SLOT(updateDashboard()));

			playerDashboard->show();
		}
		else
		{
			AI *ai = dynamic_cast<AI*> (player);

			ai->setBoardManager(boardManager);
		}
	}

	for(Field *field : fields)
	{
		if(field->getPossessable())
		{
			PossessableField *tmp = dynamic_cast<PossessableField*> (field);
			connect(tmp, SIGNAL(changeEvent(QEvent*)),
						boardDisplay, SLOT(repaint()));
		}
	}

	// Show the first player turn
	this->updateParticipantsTurn();

	// In case AI goes first
	callAI();
}

SinglePlayerGame::~SinglePlayerGame()
{
	/*
	diceDisplay->deleteLater();
	boardDisplay->deleteLater();
	fieldDisplay->deleteLater();
	*/
}

void SinglePlayerGame::setStartingPlayer()
{
	int highestDiceValue = 0;
	int playerId = -1;

	Dice *dice = boardManager->getBoard()->getDice();

	// The player with the highest dice throw goes first
	for(Player *player : boardManager->getBoard()->getPlayers())
	{
		dice->roll();

		if((dice->getFirst() + dice->getSecond()) > highestDiceValue)
		{
			highestDiceValue = dice->getFirst() + dice->getSecond();
			playerId = player->getId();
		}

		connect(player, SIGNAL(leaveGame(Player*)),
					boardManager, SLOT(removePlayer(Player*)));
	}

	boardManager->getBoard()->setPlayerTurn(playerId);

	Player *first = boardManager->getPlayer(playerId);

	NotificationManager::pushNotification("Začne " + first->getName(),
										 first->getName() + " je vrgel najvišjo"
											  " vrednost kock, zato gre prvi.",
											NotificationType::INFORMATION);
}

void SinglePlayerGame::callAI()
{
	Board *board = boardManager->getBoard();

	for(Player *player : board->getPlayers())
	{
		if((player->getPlayerType() == PlayerType::AI) &&
				(player->getId() == board->getPlayerTurn()))
		{
			AI *ai = dynamic_cast<AI*> (player);

			ai->startTurn();

			break;
		}
	}
}

void SinglePlayerGame::updateParticipantsTurn()
{
	int playerTurn = boardManager->getBoard()->getPlayerTurn();

	Player* current = boardManager->getPlayer(playerTurn);

	view->setPlayerTurn(current);
}
