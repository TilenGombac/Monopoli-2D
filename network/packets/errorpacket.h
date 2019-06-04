#ifndef ERRORPACKET_H
#define ERRORPACKET_H

#include "network/packets/packet.h"

enum class ErrorCode
{
	UNKNOWN       = 0,
	GENERAL       = 1,
	PLAYER_EXISTS = 2,
	GAME_ACTIVE   = 3,
	GAME_FULL     = 4
};

class ErrorPacket : public Packet
{
public:
	ErrorPacket(const ErrorCode &code, const QString &errorMessage);
	ErrorPacket(QByteArray data);

	ErrorCode getErrorCode();
	QString getErrorMessage();

};

#endif // ERRORPACKET_H
