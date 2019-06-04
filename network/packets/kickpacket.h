#ifndef KICKPACKET_H
#define KICKPACKET_H

#include "network/packets/packet.h"

class KickPacket : public Packet
{
public:
	KickPacket(const int &playerId, QString reason = QString());
	KickPacket(QByteArray data);

	int getPlayerId();
	QString getReason();

};

#endif // KICKPACKET_H
