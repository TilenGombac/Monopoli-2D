#ifndef OFFER_H
#define OFFER_H

#include <QObject>

#include "core/game/trade/offerpair.h"

class Player;

class Offer : public QObject
{
	Q_OBJECT

	Player *provider;
	Player *recipient;

	OfferPair provided;
	OfferPair returned;

public:
	Offer(Player *setProvider);

	Player* getProvider();
	Player* getRecipient();
	OfferPair getProvided();
	OfferPair getReturned();

	void setRecipient(Player *setRecipient);
	void setOfferPairProvided(const OfferPair &setProvided);
	void setOfferPairReturned(const OfferPair &setReturned);

public slots:
	void sendOffer();
	void acceptOffer();
	void denyOffer();

signals:
	void offerAccepted(Offer *offer);
	void offerDenied(Offer *offer);

};

#endif // OFFER_H
