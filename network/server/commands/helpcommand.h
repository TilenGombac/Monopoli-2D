#ifndef HELPCOMMAND_H
#define HELPCOMMAND_H

#include "network/server/commands/command.h"

class HelpCommand : public Command
{
public:
	HelpCommand(const QString &command);

	QString getText();

};

#endif // HELPCOMMAND_H
