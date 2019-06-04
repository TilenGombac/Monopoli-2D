#ifndef WITHDRAWPACKET_H
#define WITHDRAWPACKET_H

#include "network/packets/packet.h"

class WithdrawPacket : public Packet
{
public:
	WithdrawPacket(const int &playerId, const float &amount);
	WithdrawPacket(QByteArray data);

	int getPlayerId();
	float getAmount();

};

#endif // WITHDRAWPACKET_H
