#include "offerpair.h"

#include "core/game/entity/fields/possessablefield.h"

OfferPair::OfferPair()
{
	amount = 0;
}

OfferPair::OfferPair(const QVector<PossessableField*> &setFields,
						const float &setAmount)
{
	fields = setFields;
	amount = setAmount;
}

QVector<PossessableField*> OfferPair::getFields()
{
	return fields;
}

float OfferPair::getAmount()
{
	return amount;
}

void OfferPair::addField(PossessableField *field)
{
	if(!fields.removeOne(field))
	{
		fields.append(field);
	}
}

void OfferPair::changeAmount(const float &setAmount)
{
	amount = setAmount;
}
