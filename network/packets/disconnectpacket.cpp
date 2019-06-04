#include "disconnectpacket.h"

/*
 * Packet composition:
 * [Bytes - Part]
 *
 * [6 - Header][1 - PlayerId]
 */

DisconnectPacket::DisconnectPacket(const int &playerId)
	: Packet(PacketType::DISCONNECT)
{
	QString playerIdString = QString::number(playerId);

	this->data.append(playerIdString);
}

DisconnectPacket::DisconnectPacket(QByteArray data)
	: Packet(data)
{

}

int DisconnectPacket::getPlayerId()
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
