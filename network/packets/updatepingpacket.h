#ifndef UPDATEPINGPACKET_H
#define UPDATEPINGPACKET_H

#include "network/packets/packet.h"

class UpdatePingPacket : public Packet
{
public:
	UpdatePingPacket(const int &playerId, const int &ping);
	UpdatePingPacket(QByteArray data);

	int getPlayerId();
	int getPing();

};

#endif // UPDATEPINGPACKET_H
