#ifndef BUYPACKET_H
#define BUYPACKET_H

#include "network/packets/packet.h"

class BuyPacket : public Packet
{
public:
	BuyPacket(const int &playerId);
	BuyPacket(QByteArray data);

	int getPlayerId();

};

#endif // BUYPACKET_H
