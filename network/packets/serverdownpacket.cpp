#include "serverdownpacket.h"

/*
 * Packet composition:
 * [Bytes - Part]
 *
 * [6 - Header][X - Reason]
 */

ServerDownPacket::ServerDownPacket(QString reason)
	: Packet(PacketType::SERVER_DOWN)
{
	this->data.append(reason);
}

ServerDownPacket::ServerDownPacket(QByteArray data)
	: Packet(data)
{

}

QString ServerDownPacket::getReason()
{
	QByteArray reasonData = this->getData();

	QString reason = reasonData.mid(0);

	return reason;
}
