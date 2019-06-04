#include "pingpacket.h"

/*
 * Packet composition:
 * [Bytes - Part]
 *
 * [6 - Header][1 - Sender]
 */

PingPacket::PingPacket(const Sender &sender)
	: Packet(PacketType::PING)
{
	int senderNum = (int)sender;

	QString senderString = QString::number(senderNum);

	this->data.append(senderString);
}

PingPacket::PingPacket(QByteArray data)
	: Packet(data)
{

}

Sender PingPacket::getSender()
{
	QByteArray senderData = this->getData();

	if(senderData.size() > 0)
	{
		QString senderString = senderData.mid(0);

		bool ok;

		int senderNum = senderString.toInt(&ok);

		if(!ok)
			return Sender::UNKNOWN;

		Sender sender = (Sender)senderNum;

		return sender;
	}

	return Sender::UNKNOWN;
}
