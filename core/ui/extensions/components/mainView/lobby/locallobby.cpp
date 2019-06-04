#include "locallobby.h"

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

#include "core/game/entity/field.h"
#include "core/game/entity/tokens/automobiletoken.h"
#include "core/game/entity/tokens/battleshiptoken.h"
#include "core/game/entity/tokens/howitzertoken.h"
#include "core/game/entity/tokens/scottishterriertoken.h"
#include "core/game/entity/tokens/thimbletoken.h"
#include "core/game/entity/tokens/tophattoken.h"

#include "core/utils/errorUtils/errorhandler.h"
#include "core/utils/extensionUtils/lobbyUtils/lobbyutils.h"

#include "core/utils/playerUtils/playerutils.h"

#include "core/ui/extensions/mainview.h"
#include "core/ui/extensions/components/mainView/lobby/playerslist.h"
#include "core/ui/extensions/components/mainView/boardDisplay/boarddisplay.h"

#include "core/sounds/soundmanager.h"

LocalLobby::LocalLobby(Sidebar *sidebar, QWidget *parent)
	: Lobby(sidebar, parent)
{
	this->setGeometry(parent->geometry());
	this->initialize("NASTAVITVE LOKALNE IGRE");

	connect(addButton, SIGNAL(clicked()), this, SLOT(addPlayer()));
	connect(playerName, SIGNAL(enterPressed()), this, SLOT(addPlayer()));
	connect(tokenPicker, SIGNAL(currentIndexChanged(int)),
				this, SLOT(updateName()));
}

LocalLobby::~LocalLobby()
{
	PlayerUtils::resetPlayerId();
}

void LocalLobby::addPlayer()
{
	QString name = playerName->toPlainText();

	if((name.length() > 0) && tokenPicker->count() > 0)
	{
		Field *start = boardManager->getField(0);

		Token *token = LobbyUtils::getSelectedToken(tokenPicker, start,
														boardDisplay);

		Player *player;

		if(boardManager->getBoard()->getPlayers().count() > 0)
			player = new AI(token, name);

		else
			player = new LocalPlayer(token, name);

		this->appendPlayer(player);

		playerName->setText("");

		tokenPicker->removeItem(tokenPicker->currentIndex());

		if(tokenPicker->count() == 0)
		{
			playerName->setText("");
			playerName->setDisabled(true);
			tokenPicker->setDisabled(true);
			addButton->setDisabled(true);
		}
	}

	playersList->update();
}

void LocalLobby::startGame()
{
	int numOfPlayers = boardManager->getBoard()->getPlayers().size();
	if(numOfPlayers >= 2)
	{
		SoundManager::stopAmbient();

		this->parentWidget()->hide();

		game = new SinglePlayerGame(mainView, boardDisplay);
	}
	else
	{
		QString title = "Ni dovolj igralcev";
		QString text  = "Igra mora vsebovati od 2 do 6 igralcev, vi pa ";
		if(numOfPlayers == 0)
			text += "trenutno nimate igralcev.";
		else if(numOfPlayers == 1)
			text += "imate trenutno le enega igralca.";

		ErrorHandler::message(title, text);
	}
}

void LocalLobby::updateName()
{
	if(boardManager->getBoard()->getPlayers().size() > 0)
	{
		QString suggestedName = tokenPicker->currentText();

		suggestedName += " AI";

		playerName->setPlaceholderText("Vnesite ime računalnika");

		playerName->setText(suggestedName);
	}
}
