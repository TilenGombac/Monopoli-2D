#ifndef MOVEPACKET_H
#define MOVEPACKET_H

#include "network/packets/packet.h"

class MovePacket : public Packet
{
public:
	MovePacket(const int &playerId, const int &fieldId);
	MovePacket(QByteArray data);

	int getPlayerId();
	int getFieldId();

};

#endif // MOVEPACKET_H
