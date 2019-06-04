#ifndef ENDTURNPACKET_H
#define ENDTURNPACKET_H

#include "network/packets/packet.h"

class EndTurnPacket : public Packet
{
public:
	EndTurnPacket(const int &playerId);
	EndTurnPacket(QByteArray data);

	int getPlayerId();

};

#endif // ENDTURNPACKET_H
