#ifndef COMMANDMANAGER_H
#define COMMANDMANAGER_H

#include <QObject>

class Server;

class CommandManager : public QObject
{
	Q_OBJECT

	Server* server;

public:
	CommandManager(Server* server);

	bool execute(const QString &commandString);

signals:
	void commandExecuted(QString text, bool msg = false);

};

#endif // COMMANDMANAGER_H
