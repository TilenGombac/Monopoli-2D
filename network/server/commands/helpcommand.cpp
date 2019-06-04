#include "helpcommand.h"

HelpCommand::HelpCommand(const QString &command)
	: Command(command)
{

}

QString HelpCommand::getText()
{
	QString commandList = "Seznam vseh ukazov:\n";
	commandList += "/MSG {besedilo} -> Pošlje sporočilo vsem igralcem\n";
	commandList += "/HELP -> Seznam vseh ukazov\n";
	commandList += "? -> Seznam vseh ukazov\n";
	commandList += "/START -> Začne igro\n";
	commandList += "/STOP -> Ustavi (konča) igro\n";
	commandList += "/QUIT -> Ugasni strežnik\n";
	commandList += "/EXIT -> Ugasni strežnik\n";
	commandList += "/KICK {igralec} -> Igralca odstrani iz igre\n";
	commandList += "/DEPOSIT {vrednost} {igralec} -> Položi vrednost na igralčev račun\n";
	commandList += "/WITHDRAW {vrednost} {igralec} -> Poskusi dvigniti vrednost z igralčevega računa\n";
	commandList += "/MOVE {ID Polja} {Igralec} -> Premakne igralca na določeno polje\n";
	commandList += "/IP -> Izpiše naslov strežnika\n";
	commandList += "/ADDRESS -> Izpiše naslov strežnika";

	return commandList;
}
