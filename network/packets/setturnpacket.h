#ifndef SETTURNPACKET_H
#define SETTURNPACKET_H

#include "network/packets/packet.h"

class SetTurnPacket : public Packet
{
public:
	SetTurnPacket(const int &playerId);
	SetTurnPacket(QByteArray data);

	int getPlayerId();

};

#endif // SETTURNPACKET_H
