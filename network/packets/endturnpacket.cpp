#include "endturnpacket.h"

/*
 * Packet composition:
 * [Bytes - Part]
 *
 * [6 - Header][1 - PlayerId]
 */

EndTurnPacket::EndTurnPacket(const int &playerId)
	: Packet(PacketType::END_TURN)
{
	QString playerIdString = QString::number(playerId);

	this->data.append(playerIdString);
}

EndTurnPacket::EndTurnPacket(QByteArray data)
	: Packet(data)
{

}

int EndTurnPacket::getPlayerId()
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
