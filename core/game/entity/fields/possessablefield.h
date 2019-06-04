#ifndef POSSESSABLEFIELD_H
#define POSSESSABLEFIELD_H

#include "core/game/entity/field.h"

enum class GroupMultiply
{
	MULTIPLY_1X = 0,
	MULTIPLY_2X = 1,
	MULTIPLY_3X = 2,
	MULTIPLY_4X = 3
};

class PossessableField : public Field
{
	Q_OBJECT

public:
	PossessableField(QWidget *parent = 0);
	PossessableField(const int &id, QWidget *parent = 0);
	PossessableField(PossessableField *field, QWidget *parent = 0);

	float getPrice();
	virtual	float getRent(const bool &special) = 0;
	float getBaseRent();
	bool getMortgaged();
	int getOwnerId();
	int getGroupId();
	GroupMultiply getGroupMultiply();

	void setPrice(const float &setPrice);
	void setRent(const float &setRent);
	void setMortgaged(const bool &setMortgaged);
	void setOwnerId(const int &setOwnerId);
	void setGroupId(const int &setGroupId);
	void setGroupMultiply(const GroupMultiply &setGroupMultiply);

	QByteArray getBytes();

private:
	void initializeValues();

protected:
	float price; // base price
	float rent; // base rent - can be calculated fairly well
	bool mortgaged;
	int ownerId;
	int groupId; // Is this needed?

	GroupMultiply groupMultiply;

signals:
	void changeEvent(QEvent*);

};

#endif // POSSESSABLEFIELD_H
