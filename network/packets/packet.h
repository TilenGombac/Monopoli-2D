#ifndef PACKET_H
#define PACKET_H

#include <QString>
#include <QVector>
#include <QByteArray>

#include "network/network.h"

enum class PacketType
{
	INVALID          = -1,
	ERROR            = 0,
	PING             = 1,
	CHAT             = 2,
	CONNECT          = 3,
	DISCONNECT       = 4,
	SERVER_DOWN      = 5,
	MOVE             = 6,
	SET_TURN         = 7,
	PLAYER_ACTION    = 8,
	REQUEST_PAYMENT  = 9,
	KICK             = 10,
	START            = 11,
	STOP             = 12,
	UPDATE_BOARD     = 13,
	ROLL_DICE        = 14,
	UPDATE_DICE      = 15,
	END_TURN         = 16,
	TRADE            = 17,
	DEPOSIT          = 18,
	WITHDRAW         = 19,
	JAIL             = 20,
	BUY              = 21,
	UPDATE_DASHBOARD = 22,
	NOTIFICATION     = 23,
	UPDATE_PING      = 24
};

class Packet
{

public:
	Packet(QByteArray data);
	Packet(const PacketType &type);

	void setType(const PacketType &type);
	void setData(QByteArray data);

	PacketType getType();
	QByteArray getData(bool includeHeader = false);

	static PacketType determineType(QByteArray data);

protected:
	QByteArray data;
	PacketType type;

};

#endif // PACKET_H
