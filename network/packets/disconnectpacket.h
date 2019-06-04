#ifndef DISCONNECTPACKET_H
#define DISCONNECTPACKET_H

#include "network/packets/packet.h"

class DisconnectPacket : public Packet
{
public:
	DisconnectPacket(const int &playerId);
	DisconnectPacket(QByteArray data);

	int getPlayerId();

};

#endif // DISCONNECTPACKET_H
