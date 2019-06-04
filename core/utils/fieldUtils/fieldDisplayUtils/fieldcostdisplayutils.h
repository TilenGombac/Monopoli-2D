#ifndef FIELDCOSTDISPLAYUTILS_H
#define FIELDCOSTDISPLAYUTILS_H

class QString;
class QPainter;

class FieldCostDisplayUtils
{
public:
	static void drawCostString(QPainter *painter, QString costString,
								const bool &sidebar);

	static QString CURRENCY;
};

#endif // FIELDCOSTDISPLAYUTILS_H
