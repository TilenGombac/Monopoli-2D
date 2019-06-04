#include "buypacket.h"

/*
 * Packet composition:
 * [Bytes - Part]
 *
 * [6 - Header][1 - PlayerId]
 */

BuyPacket::BuyPacket(const int &playerId)
	: Packet(PacketType::BUY)
{
	QString playerIdString = QString::number(playerId);

	this->data.append(playerIdString);
}

BuyPacket::BuyPacket(QByteArray data)
	: Packet(data)
{

}

int BuyPacket::getPlayerId()
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
