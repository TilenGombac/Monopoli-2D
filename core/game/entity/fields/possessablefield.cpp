#include "possessablefield.h"

#include <QEvent>

PossessableField::PossessableField(QWidget *parent)
	: Field(parent)
{
	this->initializeValues();
}

PossessableField::PossessableField(const int &id, QWidget *parent)
	: Field(id, parent)
{
	this->initializeValues();
}

PossessableField::PossessableField(PossessableField *field, QWidget *parent)
	: Field(field, parent)
{
	price         = field->getPrice();
	mortgaged     = field->getMortgaged();
	ownerId       = field->getOwnerId();
	groupId       = field->getGroupId();
}

float PossessableField::getPrice()
{
	return price;
}

float PossessableField::getBaseRent()
{
	return rent;
}

bool PossessableField::getMortgaged()
{
	return mortgaged;
}

int PossessableField::getOwnerId()
{
	return ownerId;
}

int PossessableField::getGroupId()
{
	return groupId;
}

GroupMultiply PossessableField::getGroupMultiply()
{
	return groupMultiply;
}

void PossessableField::setPrice(const float &setPrice)
{
	price = setPrice;
}

void PossessableField::setRent(const float &setRent)
{
	rent = setRent;
}

void PossessableField::setMortgaged(const bool &setMortgaged)
{
	mortgaged = setMortgaged;

	emit changeEvent(new QEvent(QEvent::Type::Paint));
}

void PossessableField::setOwnerId(const int &setOwnerId)
{
	ownerId = setOwnerId;

	emit changeEvent(new QEvent(QEvent::Type::Paint));
}

void PossessableField::setGroupId(const int &setGroupId)
{
	groupId = setGroupId;
}

void PossessableField::setGroupMultiply(const GroupMultiply &setGroupMultiply)
{
	groupMultiply = setGroupMultiply;
}

QByteArray PossessableField::getBytes()
{
	return QByteArray();
}

void PossessableField::initializeValues()
{
	groupMultiply = GroupMultiply::MULTIPLY_1X;
	mortgaged     = false;
	ownerId       = -1;
	groupId       = -1;
	rent          = 0;
}
