#include "offer.h"

#include "core/game/player/player.h"

Offer::Offer(Player *setProvider)
	: QObject()
{
	provider = setProvider;
}

Player* Offer::getProvider()
{
	return provider;
}

Player* Offer::getRecipient()
{
	return recipient;
}

OfferPair Offer::getProvided()
{
	return provided;
}

OfferPair Offer::getReturned()
{
	return returned;
}

void Offer::setRecipient(Player *setRecipient)
{
	recipient = setRecipient;
}

void Offer::setOfferPairProvided(const OfferPair &setProvided)
{
	provided = setProvided;
}

void Offer::setOfferPairReturned(const OfferPair &setReturned)
{
	returned = setReturned;
}

void Offer::sendOffer()
{
	recipient->suggestTrade(this);
}

void Offer::acceptOffer()
{
	/* Provider sends the offer
	 * Recipient accepts the offer
	 * Provided pair goes to recipient
	 * Returned pair goes to provider
	 */

	float amount1 = provided.getAmount();
	float amount2 = returned.getAmount();

	if((provider->getAccountBalance() >= amount1)
			&& (recipient->getAccountBalance() >= amount2))
	{
		provider->transfer(amount1, recipient);

		recipient->transfer(amount2, provider);

		for(PossessableField *field : provided.getFields())
		{
			provider->transfer(field, recipient);
		}

		for(PossessableField *field : returned.getFields())
		{
			recipient->transfer(field, provider);
		}
	}
	else
	{
		provider->sendOfferDenied(recipient);
	}

	provider->sendOfferAccepted(recipient);

	this->deleteLater();
}

void Offer::denyOffer()
{
	provider->sendOfferDenied(recipient);

	this->deleteLater();
}
