#include "startpacket.h"

/*
 * Packet composition:
 * [Bytes - Part]
 *
 * [6 - Header]
 */

StartPacket::StartPacket()
	: Packet(PacketType::START)
{

}

StartPacket::StartPacket(QByteArray data)
	: Packet(data)
{

}
