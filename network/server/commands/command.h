#ifndef COMMAND_H
#define COMMAND_H

#include <QString>
#include <QStringList>

enum class CommandType
{
	WITHDRAW,
	DEPOSIT,
	START,
	STOP,
	HELP,
	KICK,
	MOVE,
	MSG
};

class Command
{
public:
	Command(const QString &command);

	virtual QString getText() = 0;

protected:
	QStringList list;

};

#endif // COMMAND_H
