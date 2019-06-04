#include "serverchat.h"

#include <QToolButton>

#include "network/client/client.h"
#include "network/server/server.h"
#include "core/elements/textline.h"
#include "network/packets/chatpacket.h"

ServerChat::ServerChat(Server* server, BoardManager* bm, QWidget* parent)
	: Chat(7, bm, nullptr, parent)
{
	commandManager = new CommandManager(server);

	disconnect(sendButton, SIGNAL(clicked()), this, SLOT(sendMessage()));
	disconnect(messageLine, SIGNAL(enterPressed()), this, SLOT(sendMessage()));

	connect(sendButton, SIGNAL(clicked()), this, SLOT(executeCommand()));
	connect(messageLine, SIGNAL(enterPressed()), this, SLOT(executeCommand()));

	connect(commandManager, SIGNAL(commandExecuted(QString, bool)),
				this, SLOT(addText(QString, bool)));
}

void ServerChat::addClient(Client *client)
{
	clients.append(client);

	connect(client, SIGNAL(messageReceived(Message)),
				this, SLOT(addMessage(Message)));
}

void ServerChat::sendText(Client* client, const QString &text)
{
	ChatPacket packet(playerId, text);

	client->getSocket()->write(packet.getData(true));
}

void ServerChat::sendMessage()
{
	QString message = this->messageLine->toPlainText();

	this->messageLine->setText("");

	for(Client* c : clients)
	{
		this->sendText(c, message);
	}

	Message msg(playerId, message);

	this->addMessage(msg);
}

void ServerChat::executeCommand()
{
	if(!commandManager->execute(this->messageLine->toPlainText()))
	{
		this->addText("Neveljaven ukaz!");
	}

	this->messageLine->setText("");
}

void ServerChat::addText(QString text, bool msg)
{
	if(msg)
	{
		for(Client* c : clients)
		{
			this->sendText(c, text);
		}

		Message msg(playerId, text);

		this->addMessage(msg);
	}
	else
	{
		this->messages->setText(this->messages->toPlainText() + text + "\n");

		QTextCursor cursor = this->messages->textCursor();
		cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);

		this->messages->setTextCursor(cursor);
	}
}
