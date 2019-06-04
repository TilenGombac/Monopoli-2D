#ifndef UPDATEDICEPACKET_H
#define UPDATEDICEPACKET_H

#include "network/packets/packet.h"

class UpdateDicePacket : public Packet
{
public:
	UpdateDicePacket(const int &first, const int &second);
	UpdateDicePacket(QByteArray data);

	int getFirst();
	int getSecond();

};

#endif // UPDATEDICEPACKET_H
