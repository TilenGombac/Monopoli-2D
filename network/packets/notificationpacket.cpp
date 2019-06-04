#include "notificationpacket.h"

/*
 * Packet composition:
 * [Bytes - Part]
 *
 * [6 - Header][1 - Type][X - Description]
 */

NotificationPacket::NotificationPacket(const QString &description,
										const NotificationType &type)
	: Packet(PacketType::NOTIFICATION)
{
	int typeId = (int)type;

	QString typeIdString = QString::number(typeId);

	this->data.append(typeIdString);
	this->data.append(description);
}

NotificationPacket::NotificationPacket(QByteArray data)
	: Packet(data)
{

}

QString NotificationPacket::getDescription()
{
	QByteArray notificationData = this->getData();

	QString description = notificationData.mid(1);

	return description;
}

NotificationType NotificationPacket::getType()
{
	QByteArray notificationData = this->getData();

	bool ok;

	int typeId = notificationData.mid(0, 1).toInt(&ok);

	if(!ok)
		return NotificationType::ERROR;

	NotificationType type = (NotificationType)typeId;

	return type;
}
