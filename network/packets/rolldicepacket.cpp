#include "rolldicepacket.h"

/*
 * Packet composition:
 * [Bytes - Part]
 *
 * [6 - Header][1 - PlayerId]
 */

RollDicePacket::RollDicePacket(const int &playerId)
	: Packet(PacketType::ROLL_DICE)
{
	QString playerIdString = QString::number(playerId);

	this->data.append(playerIdString);
}

RollDicePacket::RollDicePacket(QByteArray data)
	: Packet(data)
{

}

int RollDicePacket::getPlayerId()
{
	QByteArray playerData = this->getData();

	if(playerData.size() > 0)
	{
		bool ok;

		int playerId = playerData.mid(0).toInt(&ok);

		if(!ok)
			return -1;

		return playerId;
	}

	return -1;
}
