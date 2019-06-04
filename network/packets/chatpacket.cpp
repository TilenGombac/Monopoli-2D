#include "chatpacket.h"

/*
 * Packet composition:
 * [Bytes - Part]
 *
 * [6 - Header][1 - SenderId][X - Message]
 */

ChatPacket::ChatPacket(const int &senderId, const QString &message)
	: Packet(PacketType::CHAT)
{
	QString senderIdString = QString::number(senderId);

	this->data.append(senderIdString);
	this->data.append(message);
}

ChatPacket::ChatPacket(QByteArray data)
	: Packet(data)
{

}

int ChatPacket::getSenderId()
{
	QByteArray chatPacketData = this->getData();

	if(chatPacketData.size() > 1)
	{
		bool ok;

		int senderId = chatPacketData.mid(0, 1).toInt(&ok);

		if(!ok)
			return -1;

		return senderId;
	}

	return -1;
}

QString ChatPacket::getMessage()
{
	QByteArray chatPacketData = this->getData();

	if(chatPacketData.size() > 1)
	{
		QString message = chatPacketData.mid(1);

		return message;
	}

	return "";
}
