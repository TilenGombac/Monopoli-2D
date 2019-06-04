#ifndef UPDATEDASHBOARDPACKET_H
#define UPDATEDASHBOARDPACKET_H

#include "network/packets/packet.h"

enum class PlayerAction
{
	UNMORTGAGE = 0,
	ROLL_DICE  = 1,
	MORTGAGE   = 2,
	END_TURN   = 3,
	DESTRUCT   = 4,
	BUILD      = 5,
	TRADE      = 6,
	BUY        = 7,
	LEAVE_JAIL = 8
};

class UpdateDashboardPacket : public Packet
{
public:
	UpdateDashboardPacket(QVector<PlayerAction> actions);
	UpdateDashboardPacket(QByteArray data);

	QVector<PlayerAction> getActions();

};

#endif // UPDATEDASHBOARDPACKET_H
