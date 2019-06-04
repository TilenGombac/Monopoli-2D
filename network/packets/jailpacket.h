#ifndef JAILPACKET_H
#define JAILPACKET_H

#include "network/packets/packet.h"

enum class JailAction
{
	GO_TO_JAIL,
	LEAVE_JAIL
};

class JailPacket : public Packet
{
public:
	JailPacket(const int &playerId, const JailAction &action);
	JailPacket(QByteArray data);

	int getPlayerId();
	JailAction getAction();

};

#endif // JAILPACKET_H
