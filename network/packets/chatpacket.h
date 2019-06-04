#ifndef CHATPACKET_H
#define CHATPACKET_H

#include "network/packets/packet.h"

class ChatPacket : public Packet
{
public:
	ChatPacket(const int &senderId, const QString &message);
	ChatPacket(QByteArray data);

	int getSenderId();
	QString getMessage();

};

#endif // CHATPACKET_H
