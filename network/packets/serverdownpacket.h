#ifndef SERVERDOWNPACKET_H
#define SERVERDOWNPACKET_H

#include "network/packets/packet.h"

class ServerDownPacket : public Packet
{
public:
	ServerDownPacket(QString reason = QString("Strežnik ni več na voljo."));
	ServerDownPacket(QByteArray data);

	QString getReason();

};

#endif // SERVERDOWNPACKET_H
