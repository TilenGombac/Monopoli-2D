#ifndef NOTIFICATIONPACKET_H
#define NOTIFICATIONPACKET_H

#include "network/packets/packet.h"

#include "core/elements/notification.h"

class NotificationPacket : public Packet
{
public:
	NotificationPacket(const QString &description,
						const NotificationType &type);
	NotificationPacket(QByteArray data);

	QString getDescription();
	NotificationType getType();

};

#endif // NOTIFICATIONPACKET_H
