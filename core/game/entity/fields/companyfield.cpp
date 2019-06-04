#include "companyfield.h"

#include <QtMath>
#include <QPainter>

#include "core/utils/constantutils.h"
#include "core/utils/fieldUtils/fieldDisplayUtils/fielddisplayutils.h"
#include "core/utils/fieldUtils/fieldDisplayUtils/fieldcostdisplayutils.h"
#include "core/utils/fieldUtils/fieldDisplayUtils/fieldimagedisplayutils.h"
#include "core/utils/fieldUtils/fieldDisplayUtils/fieldnamedisplayutils.h"

CompanyField::CompanyField(QWidget *parent)
	: PossessableField(parent)
{
	fieldType = FieldType::COMPANY_FIELD;
}

CompanyField::CompanyField(const int &id, QWidget *parent)
	: PossessableField(id, parent)
{
	fieldType = FieldType::COMPANY_FIELD;
}

CompanyField::CompanyField(CompanyField *field, QWidget *parent)
	: PossessableField(field, parent)
{
	image = field->getImage();
}

QImage CompanyField::getImage()
{
	return image;
}


float CompanyField::getRent(const bool &utilities)
{
	/*
	 * Rules:
	 * when a player ends on one of the 2 utility fields,
	 * the player pays according to the dice value rolled.
	 * If the utilities both have the same owner,
	 * the player pays 10 times the dice value, otherwise
	 * the player pays 4 times the dice value.
	 */

	// Utility fields multiply can only be Multiply_2X
	// Otherwise, that's a railway field so we must return
	// the other rent
	if((utilities) && ((int)groupMultiply <= 1))
	{
		if(groupMultiply == GroupMultiply::MULTIPLY_1X)
			return 4;
		else
			return 10;
	}
	else
	{
		return rent * qPow(2, (int)groupMultiply);
	}
}

void CompanyField::setImage(const QImage &setImage)
{
	image = setImage;
}

QByteArray CompanyField::getBytes()
{
	return QByteArray();
}

void CompanyField::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);

	QPainter painter(this);

	QString priceString = QString::number(price);

	FieldDisplayUtils::drawField(&painter, id, sidebar);

	FieldNameDisplayUtils::drawName(&painter, name, sidebar);
	FieldImageDisplayUtils::drawImage(&painter, id, image, sidebar);
	FieldCostDisplayUtils::drawCostString(&painter, priceString, sidebar);
}
