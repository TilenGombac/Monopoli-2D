#ifndef PLAYERACTIONPACKET_H
#define PLAYERACTIONPACKET_H

#include "core/game/managers/playeractionmanager.h"

#include "network/packets/packet.h"

enum class MP_PlayerAction
{
	UNMORTGAGE,
	MORTGAGE,
	DESTRUCT,
	BUILD,
	NONE
};

class PlayerActionPacket : public Packet
{
public:
	PlayerActionPacket(const int &playerId, const int &fieldId,
						const MP_PlayerAction &action);
	PlayerActionPacket(QByteArray data);

	int getPlayerId();
	int getFieldId();
	MP_PlayerAction getPlayerAction();

};

#endif // PLAYERACTIONPACKET_H
