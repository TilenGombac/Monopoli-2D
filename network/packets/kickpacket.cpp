#include "kickpacket.h"

/*
 * Packet composition:
 * [Bytes - Part]
 *
 * [6 - Header][1 - PlayerId][X - Reason]
 */

KickPacket::KickPacket(const int &playerId, QString reason)
	: Packet(PacketType::KICK)
{
	QString playerIdString = QString::number(playerId);

	this->data.append(playerIdString);
	this->data.append(reason);
}

KickPacket::KickPacket(QByteArray data)
	: Packet(data)
{

}

int KickPacket::getPlayerId()
{
	QByteArray kickData = this->getData();

	if(kickData.size() > 0)
	{
		bool ok;

		int playerId = kickData.mid(0, 1).toInt(&ok);

		if(!ok)
			return -1;

		return playerId;
	}

	return -1;
}

QString KickPacket::getReason()
{
	QByteArray kickData = this->getData();

	if(kickData.size() > 0)
	{
		QString reason = kickData.mid(1);

		return reason;
	}

	return "";
}
