#ifndef ROLLDICEPACKET_H
#define ROLLDICEPACKET_H

#include "network/packets/packet.h"

class RollDicePacket : public Packet
{
public:
	RollDicePacket(const int &playerId);
	RollDicePacket(QByteArray data);

	int getPlayerId();

};

#endif // ROLLDICEPACKET_H
