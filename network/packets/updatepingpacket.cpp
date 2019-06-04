#include "updatepingpacket.h"

/*
 * Packet composition:
 * [Bytes - Part]
 *
 * [6 - Header][1 - PlayerId][X - Ping]
 */

UpdatePingPacket::UpdatePingPacket(const int &playerId, const int &ping)
	: Packet(PacketType::UPDATE_PING)
{
	QString playerIdString = QString::number(playerId);
	QString pingString     = QString::number(ping);

	this->data.append(playerIdString);
	this->data.append(pingString);
}

UpdatePingPacket::UpdatePingPacket(QByteArray data)
	: Packet(data)
{

}

int UpdatePingPacket::getPlayerId()
{
	QByteArray pingUpdateData = this->getData();

	if(pingUpdateData.size() > 1)
	{
		bool ok;

		int playerId = pingUpdateData.mid(0, 1).toInt(&ok);

		if(!ok)
			return -1;

		return playerId;
	}

	return -1;
}

int UpdatePingPacket::getPing()
{
	QByteArray pingUpdateData = this->getData();

	if(pingUpdateData.size() > 1)
	{
		bool ok;

		int ping = pingUpdateData.mid(1).toInt(&ok);

		if(!ok)
			return -1;

		return ping;
	}

	return -1;
}
