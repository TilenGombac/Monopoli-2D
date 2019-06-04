#include "commandmanager.h"

#include "core/game/board/board.h"
#include "network/server/server.h"
#include "network/server/commands/helpcommand.h"

CommandManager::CommandManager(Server* server)
	: QObject()
{
	this->server = server;
}

bool CommandManager::execute(const QString &commandString)
{
	QString type = commandString.split(" ")[0];

	if(type.toUpper() == "/HELP" || type == "?")
	{
		HelpCommand command(commandString);

		emit commandExecuted(command.getText());

		return true;
	}

	if(type.toUpper() == "/KICK")
	{
		int nameLength = commandString.length() - type.length() - 1;
		QString playerName = commandString.right(nameLength);

		BoardManager* manager = server->getBoardManager();

		for(Player* player : manager->getBoard()->getPlayers())
		{
			if(player->getName() == playerName)
			{
				server->removePlayer(player->getId());

				emit commandExecuted(playerName + " je zapustil igro.", true);

				return true;
			}
		}

		emit commandExecuted("Ta igralec ne obstaja.");

		return true;
	}

	if(type.toUpper() == "/MSG")
	{
		int msgLength = commandString.length() - type.length() - 1;

		QString msg = commandString.right(msgLength);

		emit commandExecuted(msg, true);

		return true;
	}

	if(type.toUpper() == "/START")
	{
		if(server->getBoardManager()->getBoard()->getPlayers().count() >= 2)
		{
			if(server->startGame())
			{
				emit commandExecuted("Igra se je začela!", true);
			}
			else
			{
				emit commandExecuted("Igra že teče!");
			}
		}
		else
		{
			emit commandExecuted("Premalo igralcev!");
		}

		return true;
	}

	if(type.toUpper() == "/STOP")
	{
		if(server->stopGame())
		{
			emit commandExecuted("Igra se je končala!", true);

			this->server->parent()->deleteLater();
		}
		else
		{
			emit commandExecuted("Igra se še ni začela!");
		}

		return true;
	}

	if(type.toUpper() == "/DEPOSIT")
	{
		QString amountString = commandString.split(" ")[1];
		int nameLength = commandString.length() - type.length() - amountString.length() - 2;

		QString playerName = commandString.right(nameLength);

		BoardManager* manager = server->getBoardManager();

		for(Player* player : manager->getBoard()->getPlayers())
		{
			if(player->getName() == playerName)
			{
				bool ok;

				float amount = amountString.toFloat(&ok);

				if(!ok)
				{
					emit commandExecuted("Prišlo je do napake!");

					return true;
				}

				this->server->deposit(amount, player->getId());

				emit commandExecuted("Položeno!");

				return true;
			}
		}

		emit commandExecuted("Ta igralec ne obstaja");

		return true;
	}

	if(type.toUpper() == "/WITHDRAW")
	{
		QString amountString = commandString.split(" ")[1];
		int nameLength = commandString.length() - type.length() - amountString.length() - 2;

		QString playerName = commandString.right(nameLength);

		BoardManager* manager = server->getBoardManager();

		for(Player* player : manager->getBoard()->getPlayers())
		{
			if(player->getName() == playerName)
			{
				bool ok;

				float amount = amountString.toFloat(&ok);

				if(!ok)
				{
					emit commandExecuted("Prišlo je do napake!");

					return true;
				}

				if(this->server->withdraw(amount, player->getId()))
				{
					emit commandExecuted("Dvignjeno!");

					return true;
				}

				else
				{
					emit commandExecuted("Igralec nima zadostnih sredstev!");

					return true;
				}
			}
		}

		emit commandExecuted("Ta igralec ne obstaja");

		return true;
	}

	if(type.toUpper() == "/MOVE")
	{
		QString fieldIdString = commandString.split(" ")[1];
		int nameLength = commandString.length() - type.length() - fieldIdString.length() - 2;

		QString playerName = commandString.right(nameLength);

		BoardManager* manager = server->getBoardManager();

		for(Player* player : manager->getBoard()->getPlayers())
		{
			if(player->getName() == playerName)
			{
				bool ok;

				int fieldId = fieldIdString.toInt(&ok);

				if(!ok)
				{
					emit commandExecuted("Prišlo je do napake!");

					return true;
				}

				this->server->move(fieldId, player->getId());

				emit commandExecuted("Ukaz je bil uspešno izvršen!");

				return true;
			}
		}

		emit commandExecuted("Ta igralec ne obstaja");

		return true;
	}

	if(type.toUpper() == "/QUIT" || type.toUpper() == "/EXIT")
	{
		// Delete Server's parent (ServerWidget),
		// which will trigger the server's destructor as well

		this->server->parent()->deleteLater();

		return true;
	}

	if(type.toUpper() == "/IP" || type.toUpper() == "/ADDRESS")
	{
		QString address = this->server->getAddress().toString();

		emit commandExecuted("Naslov strežnika: " + address);

		return true;
	}

	return false;
}
