#ifndef TRADEPACKET_H
#define TRADEPACKET_H

#include "network/packets/packet.h"

#include "core/game/trade/offer.h"

class Field;

enum class TradeStatus
{
	ACCEPTED,
	PENDING,
	DENIED
};

class TradePacket : public Packet
{
public:
	TradePacket();
	TradePacket(Offer* offer, const TradeStatus &status);
	TradePacket(QByteArray data);

	void setStatus(const TradeStatus &status);

	TradeStatus getStatus();

	int getProviderId();
	int getRecipientId();

	OfferPair getProvided(QVector<Field*> fields);
	OfferPair getReturned(QVector<Field*> fields);

};

#endif // TRADEPACKET_H
