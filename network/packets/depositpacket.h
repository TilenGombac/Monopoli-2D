#ifndef DEPOSITPACKET_H
#define DEPOSITPACKET_H

#include "network/packets/packet.h"

class DepositPacket : public Packet
{
public:
	DepositPacket(const int &playerId, const float &amount);
	DepositPacket(QByteArray data);

	int getPlayerId();
	float getAmount();

};

#endif // DEPOSITPACKET_H
