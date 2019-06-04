#include "lobby.h"

#include <QLabel>
#include <QComboBox>
#include <QPushButton>

#include "core/utils/constantutils.h"
#include "core/elements/textline.h"
#include "core/ui/extensions/components/mainView/home.h"
#include "core/game/board/board.h"
#include "core/ui/extensions/sidebar.h"
#include "core/game/managers/boardmanager.h"
#include "core/game/entity/token.h"
#include "core/game/player/player.h"
#include "core/game/managers/fieldsmanager.h"
#include "core/game/managers/cardsmanager.h"
#include "core/utils/extensionUtils/lobbyUtils/lobbyutils.h"
#include "core/ui/extensions/mainview.h"
#include "core/ui/extensions/components/mainView/lobby/playerslist.h"
#include "core/ui/extensions/components/mainView/boardDisplay/boarddisplay.h"

Lobby::Lobby(Sidebar *setSidebar, QWidget *parent)
	: QWidget(parent)
{
	sidebar      = setSidebar;
	boardManager = new BoardManager();

	// First parent widget is home, it's parent is then MainView
	QWidget *mainViewWidget = this->parentWidget()->parentWidget();
	mainView = dynamic_cast<MainView*> (mainViewWidget);

	boardDisplay = new BoardDisplay(boardManager, mainView);

	connect(boardManager, SIGNAL(gameOver(Player*)),
				mainView, SLOT(endGameScreen(Player*)));
}

Lobby::~Lobby()
{
	this->hide();

	boardManager->deleteLater();
	boardDisplay->deleteLater();
}

void Lobby::initialize(const QString &lobbyName)
{
	this->initializeLobbyTitle(lobbyName);
	this->initializePlayerName();
	this->initializeTokenPicker();
	this->initializeButtons();

	boardManager->getBoard()->setFields(
				FieldsManager::getDefaultFields(boardDisplay));

	boardManager->getBoard()->setCards(
						CardsManager::getDefaultCards(boardDisplay));

	playersList = new PlayersList(boardManager, this);

	playerName->setLengthLimit(18);

	playerName->setFocus();

	this->initializeGeometry();
}

void Lobby::initializeGeometry()
{
	addButton = new QPushButton("Dodaj", this);
	addButton->setGeometry(50, 310, 300, 40);
	LobbyUtils::setFont(addButton);

	playerName->setGeometry(50, 130, 300, 40);
	tokenPicker->setGeometry(50, 220, 300, 40);
}

void Lobby::appendPlayer(Player *player)
{
	QVector<Player*> players = boardManager->getBoard()->getPlayers();

	players.append(player);

	boardManager->getBoard()->setPlayers(players);
}

void Lobby::initializeLobbyTitle(const QString &lobbyName)
{
	lobbyTitle = new QLabel(lobbyName, this);

	int fieldWidth = ConstantUtils::FIELD_WIDTH;

	lobbyTitle->setGeometry(50, 20, this->width(), 60);
	lobbyTitle->setFont(QFont("Arial Black", fieldWidth / 2));
	lobbyTitle->setAlignment(Qt::AlignTop);
}

void Lobby::initializePlayerName()
{
	playerName = new TextLine(this);

	playerName->setPlaceholderText("Vnesite svoje ime");
	playerName->setAlignment(Qt::AlignVCenter);

	LobbyUtils::setFont(playerName);
}

void Lobby::initializeTokenPicker()
{
	tokenPicker = new QComboBox(this);

	LobbyUtils::initializeTokenPicker(tokenPicker);
	LobbyUtils::setFont(tokenPicker);
}

void Lobby::initializeButtons()
{
	startGameBtn = new QPushButton("ZAČNI IGRO", this);
	quitLobbyBtn = new QPushButton("Zapri", this);

	LobbyUtils::initializeStartButton(startGameBtn);
	LobbyUtils::initializeQuitButton(quitLobbyBtn);

	connect(quitLobbyBtn, SIGNAL(clicked()),
				this, SLOT(quitLobby()));
	connect(startGameBtn, SIGNAL(clicked()),
				this, SLOT(startGame()));
}

void Lobby::quitLobby()
{
	Home *home = mainView->getHome();

	home->initialize();

	this->deleteLater();
}
