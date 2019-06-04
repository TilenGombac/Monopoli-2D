#ifndef SERVERCHAT_H
#define SERVERCHAT_H

#include "network/server/commands/commandmanager.h"
#include "core/ui/extensions/components/sidebar/chat.h"

class Server;

class ServerChat : public Chat
{
	Q_OBJECT

	CommandManager* commandManager;

	QVector<Client*> clients;

public:
	ServerChat(Server* server, BoardManager* bm, QWidget* parent = 0);

	void addClient(Client* client);

	void sendText(Client* client, const QString &text);

private slots:
	void sendMessage();
	void executeCommand();

public slots:
	void addText(QString text, bool msg = false);

};

#endif // SERVERCHAT_H
