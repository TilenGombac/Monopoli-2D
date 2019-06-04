#include "updatedicepacket.h"

/*
 * Packet composition:
 * [Bytes - Part]
 *
 * [6 - Header][1 - First][1 - Second]
 */

UpdateDicePacket::UpdateDicePacket(const int &first, const int &second)
	: Packet(PacketType::UPDATE_DICE)
{
	QString firstString  = QString::number(first);
	QString secondString = QString::number(second);

	this->data.append(firstString);
	this->data.append(secondString);
}

UpdateDicePacket::UpdateDicePacket(QByteArray data)
	: Packet(data)
{

}

int UpdateDicePacket::getFirst()
{
	QByteArray diceData = this->getData();
	if(diceData.size() > 1)
	{
		bool ok;

		int first = diceData.mid(0, 1).toInt(&ok);

		if(!ok)
			return -1;

		return first;
	}

	return -1;
}

int UpdateDicePacket::getSecond()
{
	QByteArray diceData = this->getData();
	if(diceData.size() > 1)
	{
		bool ok;

		int second = diceData.mid(1, 1).toInt(&ok);

		if(!ok)
			return -1;

		return second;
	}

	return -1;
}
