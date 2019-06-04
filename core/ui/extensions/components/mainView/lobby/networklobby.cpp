#include "networklobby.h"

#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QMessageBox>

#include "core/elements/textline.h"

#include "core/game/player/ai.h"
#include "core/game/board/board.h"
#include "core/game/singleplayergame.h"
#include "core/game/player/localplayer.h"
#include "core/game/managers/boardmanager.h"

#include "core/game/entity/fields/possessablefield.h"

#include "core/game/managers/boardmanager.h"

#include "core/game/entity/field.h"
#include "core/game/entity/tokens/automobiletoken.h"
#include "core/game/entity/tokens/battleshiptoken.h"
#include "core/game/entity/tokens/howitzertoken.h"
#include "core/game/entity/tokens/scottishterriertoken.h"
#include "core/game/entity/tokens/thimbletoken.h"
#include "core/game/entity/tokens/tophattoken.h"

#include "core/utils/errorUtils/errorhandler.h"
#include "core/utils/extensionUtils/lobbyUtils/lobbyutils.h"

#include "core/ui/extensions/mainview.h"
#include "core/ui/extensions/sidebar.h"
#include "core/ui/extensions/components/sidebar/chat.h"
#include "core/ui/extensions/components/sidebar/participantsview.h"
#include "core/ui/extensions/components/mainView/boardDisplay/boarddisplay.h"

#include "core/game/mpgame.h"

#include "core/ui/extensions/components/mainView/lobby/playerslist.h"

#include "network/client/localclient.h"
#include "network/packets/connectpacket.h"
#include "network/packets/disconnectpacket.h"

#include "core/sounds/soundmanager.h"

NetworkLobby::NetworkLobby(Sidebar *sidebar, QHostAddress serverAddress,
							QWidget *parent)
	: Lobby(sidebar, parent)
{
	this->setGeometry(parent->geometry());
	this->initialize("NASTAVITVE OMREŽNE IGRE");

	addButton->setText("Pridruži se");

	connect(addButton, SIGNAL(clicked()), this, SLOT(addPlayer()));
	connect(playerName, SIGNAL(enterPressed()), this, SLOT(addPlayer()));

	startGameBtn->hide();

	client = new LocalClient(serverAddress, this);

	participants = new ParticipantsView(true, sidebar);

	playersList->hide();

	player = nullptr;

	connect(client, SIGNAL(playerExists()),
				this, SLOT(enablePlayerEntry()));

	connect(client, SIGNAL(gameFull()),
				this, SLOT(quitLobby()));

	connect(client, SIGNAL(playerDisconnected(int)),
				this, SLOT(playerDisconnected(int)));

	connect(client, SIGNAL(playerConnected(NetworkPlayer*)),
				this, SLOT(playerConnected(NetworkPlayer*)));

	connect(client, SIGNAL(boardUpdated(UpdateBoardPacket)),
				this, SLOT(updateBoard(UpdateBoardPacket)));

	connect(client, SIGNAL(serverOffline(QString)),
				this, SLOT(onServerDown(QString)));

	connect(client, SIGNAL(gameStarted()), this, SLOT(startGame()));

	connect(client, SIGNAL(gameStopped()),
				this, SLOT(quitLobby()));

	connect(client, SIGNAL(turnChanged(int)),
				this, SLOT(setPlayerTurn(int)));

	connect(client, SIGNAL(updatePing(int,int)),
				participants, SLOT(refreshPing(int,int)));
}

NetworkLobby::~NetworkLobby()
{
	if(player != nullptr)
	{
		DisconnectPacket disconnectPacket(player->getId());

		client->getSocket()->write(disconnectPacket.getData(true));

		if(client->getSocket()->waitForBytesWritten())
		{
			client->getSocket()->disconnectFromHost();
		}

		chat->deleteLater();
	}

	participants->deleteLater();
}

void NetworkLobby::removeFromTokenPicker(const QString &tokenName)
{
	for(int i = 0; i < tokenPicker->count(); i++)
	{
		if(tokenName == tokenPicker->itemText(i))
		{
			tokenPicker->removeItem(i);

			return;
		}
	}
}

void NetworkLobby::enablePlayerEntry()
{
	playerName->setDisabled(false);
	tokenPicker->setDisabled(false);
	addButton->setDisabled(false);
}

void NetworkLobby::addPlayer()
{
	QString name = playerName->toPlainText();

	if(name.length() > 0)
	{
		Field *start = boardManager->getField(0);

		Token *token = LobbyUtils::getSelectedToken(tokenPicker, start,
														boardDisplay);

		// 9 is the uninitialized playerID
		player = new NetworkPlayer(token, name, 9);

		ConnectPacket packet(player);

		client->getSocket()->write(packet.getData(true));

		playerName->setText("");
		playerName->setDisabled(true);
		tokenPicker->setDisabled(true);
		addButton->setDisabled(true);
	}
}

void NetworkLobby::startGame()
{
	if(player != nullptr)
	{
		SoundManager::stopAmbient();

		this->parentWidget()->hide();

		new MPGame(client, mainView, boardDisplay, participants, player);
	}
}

void NetworkLobby::onServerDown(QString reason)
{
	ErrorHandler::message("Napaka", reason);

	this->quitLobby();
}

void NetworkLobby::playerConnected(NetworkPlayer *connectedPlayer)
{
	if(connectedPlayer == nullptr)
		return;

	// A default new CornerField(0) is called in ConnectPacket
	// getPlayer, which we must delete to prevent a memory leak
	Field* tmp = connectedPlayer->getToken()->getField();
	tmp->deleteLater();

	connectedPlayer->getToken()->setField(boardManager->getField(0));
	connectedPlayer->getToken()->setParent(boardDisplay);

	if(player != nullptr)
	{
		if((player->getId() == 9) &&
				(player->getName() == connectedPlayer->getName()))
		{
			delete player;

			player = connectedPlayer;

			chat = new Chat(player->getId(), this->boardManager,
								this->client, this->sidebar);
		}
	}

	participants->addParticipant((Player*)connectedPlayer);

	this->appendPlayer((Player*)connectedPlayer);

	QVector<Player*> participatingPlayers = participants->getParticipants();

	for(Player *participant : participatingPlayers)
	{
		TokenType tokenType = participant->getToken()->getTokenType();

		if(tokenType == TokenType::AUTOMOBILE)
			this->removeFromTokenPicker("Avtomobil");
		else if(tokenType == TokenType::BATTLESHIP)
			this->removeFromTokenPicker("Bojna ladja");
		else if(tokenType == TokenType::HOWITZER)
			this->removeFromTokenPicker("Top");
		else if(tokenType == TokenType::SCOTTISH_TERRIER)
			this->removeFromTokenPicker("Škotski terier");
		else if(tokenType == TokenType::THIMBLE)
			this->removeFromTokenPicker("Naprstnik");
		else
			this->removeFromTokenPicker("Klobuk");
	}
}

void NetworkLobby::playerDisconnected(int playerId)
{
	Player* leaver = boardManager->getPlayer(playerId);

	// Transfer all player fields back to the bank when the player leaves
	if(leaver != nullptr)
	{
		for(Field* field : leaver->getFields())
		{
			PossessableField* tmp = (PossessableField*)field;

			tmp->setOwnerId(-1);
		}
	}

	if(player->getId() == playerId)
	{
		this->quitLobby();

		return;
	}

	for(Player* participant : participants->getParticipants())
	{
		if(participant->getId() == playerId)
		{
			participants->removeParticipant(participant);

			return;
		}
	}

	leaver->deleteLater();
}

void NetworkLobby::setPlayerTurn(int playerId)
{
	this->boardManager->getBoard()->setPlayerTurn(playerId);

	Player* player = this->boardManager->getPlayer(playerId);

	participants->setPlayerTurn(player);
}

void NetworkLobby::updateBoard(UpdateBoardPacket boardPacket)
{
	Board* board = this->boardManager->getBoard();

	boardPacket.getUpdatedBoard(board);

	int playerId = board->getPlayerTurn();

	Player* player = this->boardManager->getPlayer(playerId);

	participants->setPlayerTurn(player);
}
