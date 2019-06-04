#ifndef STARTPACKET_H
#define STARTPACKET_H

#include "network/packets/packet.h"

class StartPacket : public Packet
{
public:
	StartPacket();
	StartPacket(QByteArray data);

};

#endif // STARTPACKET_H
