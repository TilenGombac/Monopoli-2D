#ifndef PROPERTYFIELD_H
#define PROPERTYFIELD_H

#include "core/game/entity/fields/possessablefield.h"

// Used to determine the rent value
enum class PropertyBuildings
{
	HOUSE_0X = 0,
	HOUSE_1X = 1,
	HOUSE_2X = 2,
	HOUSE_3X = 3,
	HOUSE_4X = 4,
	HOTEL    = 5
};

class PropertyField : public PossessableField
{
	QColor color; // rectangle color of property
	PropertyBuildings propertyBuildings;

public:
	PropertyField(QWidget *parent = 0);
	PropertyField(const int &id, QWidget *parent = 0);
	PropertyField(PropertyField *field, QWidget *parent = 0);

	QColor getColor();
	PropertyBuildings getBuildings();

	float getRent(const bool &doubleBase);

	void setColor(const QColor &setColor);
	void setBuildings(const PropertyBuildings &setBuildings);

	QByteArray getBytes();

protected:
	void paintEvent(QPaintEvent *event);
};

#endif // PROPERTYFIELD_H
