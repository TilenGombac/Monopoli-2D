#include "chat.h"

#include <QTextEdit>
#include <QToolButton>

#include "network/client/client.h"
#include "core/game/board/board.h"
#include "core/elements/textline.h"
#include "core/game/player/player.h"
#include "core/utils/constantutils.h"
#include "network/packets/chatpacket.h"
#include "core/game/managers/boardmanager.h"

Chat::Chat(const int &playerId, BoardManager* bm, Client* client,
			QWidget *parent)
	: QWidget(parent)
{
	if(ConstantUtils::FIELD_WIDTH < 80)
	{
		this->setWindowFlags(Qt::Window);
	}

	this->playerId     = playerId;
	this->boardManager = bm;
	this->client       = client;

	messages    = new QTextEdit(this);
	messageLine = new TextLine(this);
	sendButton  = new QToolButton(this);

	messages->setFont(QFont("Verdana", 13));
	messageLine->setFont(QFont("Verdana", 13));

	if(parent != nullptr)
	{
		this->resetGeometry();
	}

	if(parent != nullptr)
	{
		connect(sendButton, SIGNAL(clicked()), this, SLOT(sendMessage()));
		connect(messageLine, SIGNAL(enterPressed()), this, SLOT(sendMessage()));

		connect(client, SIGNAL(messageReceived(Message)),
					this, SLOT(addMessage(Message)));
	}
}

void Chat::resetGeometry()
{
	this->setGeometry(1, 280, this->parentWidget()->width() - 2, 230);

	this->setFixedSize(this->size());

	int width   = this->width();
	int height  = this->height();

	messages->setGeometry(0, 0, width, (height - 30));
	messageLine->setGeometry(0, (height - 30), (width - 60), 30);
	sendButton->setGeometry((width - 60), (height - 30), 60, 30);

	messageLine->setPlaceholderText("Vnesi sporočilo...");
	sendButton->setText("Pošlji");

	messages->show();
	messageLine->show();
	sendButton->show();

	messages->setReadOnly(true);

	this->show();
}

void Chat::addMessage(Message msg)
{
	int senderId = msg.getSenderId();

	// It's always from the server if it's not from a player
	QString senderName = "Strežnik";

	for(Player* player : boardManager->getBoard()->getPlayers())
	{
		if(player->getId() == senderId)
		{
			senderName = player->getName();

			break;
		}
	}

	QString currentMessages = messages->toPlainText();

	QString msgText = msg.getMessage(senderName);

	messages->setText(currentMessages + msgText);

	QTextCursor cursor = messages->textCursor();
	cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);

	messages->setTextCursor(cursor);
}

void Chat::sendMessage()
{
	QString message = this->messageLine->toPlainText();

	ChatPacket packet(playerId, message);

	client->getSocket()->write(packet.getData(true));

	this->messageLine->setText("");
}
