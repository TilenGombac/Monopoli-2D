#ifndef REQUESTPAYMENTPACKET_H
#define REQUESTPAYMENTPACKET_H

#include "network/packets/packet.h"

class RequestPaymentPacket : public Packet
{
public:
	RequestPaymentPacket(const int &playerId, const float &amount);
	RequestPaymentPacket(QByteArray data);

	int getPlayerId();
	float getAmount();

};

#endif // REQUESTPAYMENTPACKET_H
