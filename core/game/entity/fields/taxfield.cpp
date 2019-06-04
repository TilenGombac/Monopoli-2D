#include "taxfield.h"

#include <QPainter>

#include "core/utils/fieldUtils/fieldDisplayUtils/fielddisplayutils.h"
#include "core/utils/fieldUtils/fieldDisplayUtils/fieldcostdisplayutils.h"
#include "core/utils/fieldUtils/fieldDisplayUtils/fieldimagedisplayutils.h"
#include "core/utils/fieldUtils/fieldDisplayUtils/fieldnamedisplayutils.h"

TaxField::TaxField(QWidget *parent)
	: Field(parent)
{
	fieldType = FieldType::TAX_FIELD;
}

TaxField::TaxField(const int &id, QWidget *parent)
	: Field(id, parent)
{
	fieldType = FieldType::TAX_FIELD;
}

TaxField::TaxField(TaxField *field, QWidget *parent)
	: Field(field, parent)
{
	payString = field->getPayString();
	image     = field->getImage();
	cost      = field->getCost();
}

QString TaxField::getPayString()
{
	return payString;
}

QImage TaxField::getImage()
{
	return image;
}

float TaxField::getCost()
{
	return cost;
}

void TaxField::setPayString(const QString &setPayString)
{
	payString = setPayString;
}

void TaxField::setImage(const QImage &setImage)
{
	image = setImage;
}

void TaxField::setCost(const float &setCost)
{
	cost = setCost;
}

QByteArray TaxField::getBytes()
{
	QByteArray data;

	return data;
}

void TaxField::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);

	QPainter painter(this);

	QString costString = payString + " " + QString::number(cost);

	FieldDisplayUtils::drawField(&painter, id, sidebar);

	FieldNameDisplayUtils::drawName(&painter, name, sidebar);
	FieldImageDisplayUtils::drawImage(&painter, id, image, sidebar);
	FieldCostDisplayUtils::drawCostString(&painter, costString, sidebar);
}
