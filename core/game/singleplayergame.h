#ifndef SINGLEPLAYERGAME_H
#define SINGLEPLAYERGAME_H

#include <QObject>

class Dice;
class Sidebar;
class MainView;
class DiceDisplay;
class BoardDisplay;
class BoardManager;
class FieldDisplay;
class PlayerDashboard;
class ParticipantsView;

class SinglePlayerGame : public QObject
{
	Q_OBJECT

	BoardDisplay *boardDisplay;
	BoardManager *boardManager;

	Sidebar *sidebar;

	DiceDisplay  *diceDisplay;
	FieldDisplay *fieldDisplay;

	PlayerDashboard *playerDashboard;

	ParticipantsView* view;

public:
	SinglePlayerGame(MainView *mainView, BoardDisplay *setBoardDisplay);
	~SinglePlayerGame();

	void setStartingPlayer();

public slots:
	void callAI();

private slots:
	void updateParticipantsTurn();

};

#endif // SINGLEPLAYERGAME_H
