#include "command.h"

Command::Command(const QString &command)
{
	list = command.split(" ");
}
