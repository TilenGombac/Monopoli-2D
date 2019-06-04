#include "propertyfield.h"

#include <QPainter>

#include "core/utils/buildingUtils/buildingdisplayutils.h"
#include "core/utils/fieldUtils/fieldDisplayUtils/fielddisplayutils.h"
#include "core/utils/fieldUtils/fieldDisplayUtils/fieldcolordisplayutils.h"
#include "core/utils/fieldUtils/fieldDisplayUtils/fieldcostdisplayutils.h"
#include "core/utils/fieldUtils/fieldDisplayUtils/fieldnamedisplayutils.h"

PropertyField::PropertyField(QWidget *parent)
	: PossessableField(parent)
{
	fieldType         = FieldType::PROPERTY_FIELD;
	propertyBuildings = PropertyBuildings::HOUSE_0X;
}

PropertyField::PropertyField(const int &id, QWidget *parent)
	: PossessableField(id, parent)
{
	fieldType         = FieldType::PROPERTY_FIELD;
	propertyBuildings = PropertyBuildings::HOUSE_0X;
}

PropertyField::PropertyField(PropertyField *field, QWidget *parent)
	: PossessableField(field, parent)
{
	color             = field->getColor();
	propertyBuildings = field->getBuildings();
}

QColor PropertyField::getColor()
{
	return color;
}

PropertyBuildings PropertyField::getBuildings()
{
	return propertyBuildings;
}

float PropertyField::getRent(const bool &doubleBase)
{
	if(propertyBuildings == PropertyBuildings::HOUSE_1X)
		return rent * 5;

	if(propertyBuildings == PropertyBuildings::HOUSE_2X)
		return rent * 15;

	if(propertyBuildings == PropertyBuildings::HOUSE_3X)
		return rent * 35;

	if(propertyBuildings == PropertyBuildings::HOUSE_4X)
		return rent * 45;

	if(propertyBuildings == PropertyBuildings::HOTEL)
		return rent * 55;

	if(doubleBase)
		return rent * 2;

	return rent;
}

void PropertyField::setColor(const QColor &setColor)
{
	color = setColor;
}

void PropertyField::setBuildings(const PropertyBuildings &setBuildings)
{
	propertyBuildings = setBuildings;

	if(propertyBuildings > PropertyBuildings::HOUSE_0X)
	{
		Action tmpAction;

		tmpAction.setPayAction(this->getRent(false));

		this->setAction(tmpAction);
	}

	emit clicked();

	this->repaint();
}

QByteArray PropertyField::getBytes()
{
	return QByteArray();
}

void PropertyField::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);

	QPainter painter(this);

	QString priceString = QString::number(price);

	FieldDisplayUtils::drawField(&painter, id, sidebar);

	FieldColorDisplayUtils::drawColorBox(&painter, color, sidebar);
	FieldNameDisplayUtils::drawName(&painter, name, sidebar, true);
	FieldCostDisplayUtils::drawCostString(&painter, priceString, sidebar);

	BuildingDisplayUtils::drawBuildings(&painter, (int)propertyBuildings,
											sidebar);
}
