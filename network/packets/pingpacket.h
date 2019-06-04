#ifndef PINGPACKET_H
#define PINGPACKET_H

#include "network/packets/packet.h"

enum class Sender
{
	UNKNOWN = 0,
	SERVER  = 1,
	CLIENT  = 2
};

class PingPacket : public Packet
{
public:
	PingPacket(const Sender &sender);
	PingPacket(QByteArray data);

	Sender getSender();

};

#endif // PINGPACKET_H
