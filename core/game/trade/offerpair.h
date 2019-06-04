#ifndef OFFERPAIR_H
#define OFFERPAIR_H

#include <QVector>

class PossessableField;

class OfferPair
{
	QVector<PossessableField*> fields;
	float amount;

public:
	OfferPair();
	OfferPair(const QVector<PossessableField*> &setFields,
				const float &setAmount);

	QVector<PossessableField*> getFields();
	float getAmount();

	void addField(PossessableField *field);

	void changeAmount(const float &setAmount);

};

#endif // OFFERPAIR_H
