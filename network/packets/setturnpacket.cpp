#include "setturnpacket.h"

/*
 * Packet composition:
 * [Bytes - Part]
 *
 * [6 - Header][1 - PlayerId]
 */

SetTurnPacket::SetTurnPacket(const int &playerId)
	: Packet(PacketType::SET_TURN)
{
	QString playerIdString = QString::number(playerId);

	this->data.append(playerIdString);
}

SetTurnPacket::SetTurnPacket(QByteArray data)
	: Packet(data)
{

}

int SetTurnPacket::getPlayerId()
{
	QByteArray playerData = this->getData();

	if(playerData.size() > 0)
	{
		bool ok;

		int playerId = playerData.mid(0, 1).toInt(&ok);

		if(!ok)
			return -1;

		return playerId;
	}

	return -1;
}
