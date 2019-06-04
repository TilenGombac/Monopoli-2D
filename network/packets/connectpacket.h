#ifndef CONNECTPACKET_H
#define CONNECTPACKET_H

#include "network/packets/packet.h"

class NetworkPlayer;

class ConnectPacket : public Packet
{
public:
	ConnectPacket(NetworkPlayer *player = nullptr);
	ConnectPacket(QByteArray data);

	NetworkPlayer *getPlayer();

};

#endif // CONNECTPACKET_H
