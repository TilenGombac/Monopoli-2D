#ifndef STOPPACKET_H
#define STOPPACKET_H

#include "network/packets/packet.h"

class StopPacket : public Packet
{
public:
	StopPacket();
	StopPacket(QByteArray data);

};

#endif // STOPPACKET_H
