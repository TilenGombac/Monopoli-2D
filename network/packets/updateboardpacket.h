#ifndef UPDATEBOARDPACKET_H
#define UPDATEBOARDPACKET_H

#include "network/packets/packet.h"

class Board;

class UpdateBoardPacket : public Packet
{
public:
	UpdateBoardPacket(Board *board = nullptr);
	UpdateBoardPacket(QByteArray data);

	bool getUpdatedBoard(Board *current);

};

#endif // UPDATEBOARDPACKET_H
