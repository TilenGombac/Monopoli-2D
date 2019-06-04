#ifndef LOBBY_H
#define LOBBY_H

#include <QWidget>
#include <QString>

class Home;
class Board;
class QLabel;
class Player;
class Sidebar;
class TextLine;
class MainView;
class QComboBox;
class PlayersList;
class QPushButton;
class BoardManager;
class BoardDisplay;

class Lobby : public QWidget
{
	Q_OBJECT

public:
	Lobby(Sidebar *setSidebar, QWidget *parent = 0);
	virtual ~Lobby();

	void initialize(const QString &lobbyName);

	void initializeGeometry();

	void appendPlayer(Player *player);

private:
	void initializeLobbyTitle(const QString &lobbyName);
	void initializePlayerName();
	void initializeTokenPicker();
	void initializeButtons();

public slots:
	void quitLobby();
	virtual void startGame() = 0;

protected:
	Sidebar *sidebar;

	BoardManager *boardManager;

	MainView *mainView;

	BoardDisplay *boardDisplay;

	QLabel *lobbyTitle;

	TextLine *playerName;

	PlayersList *playersList;

	QComboBox *tokenPicker;

	QPushButton *addButton;

	QPushButton *startGameBtn;
	QPushButton *quitLobbyBtn;

};

#endif // LOBBY_H
