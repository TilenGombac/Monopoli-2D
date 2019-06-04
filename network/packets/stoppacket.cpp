#include "stoppacket.h"

/*
 * Packet composition:
 * [Bytes - Part]
 *
 * [6 - Header]
 */

StopPacket::StopPacket()
	: Packet(PacketType::STOP)
{

}

StopPacket::StopPacket(QByteArray data)
	: Packet(data)
{

}
