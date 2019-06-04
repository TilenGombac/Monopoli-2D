#include "cardfield.h"

#include <QPainter>

#include "core/utils/fieldUtils/fieldDisplayUtils/fielddisplayutils.h"
#include "core/utils/fieldUtils/fieldDisplayUtils/fieldimagedisplayutils.h"
#include "core/utils/fieldUtils/fieldDisplayUtils/fieldnamedisplayutils.h"

CardField::CardField(QWidget *parent)
	: Field(parent)
{
	fieldType = FieldType::CARD_FIELD;
}

CardField::CardField(const int &id, QWidget *parent)
	: Field(id, parent)
{
	fieldType = FieldType::CARD_FIELD;
}

CardField::CardField(CardField *field, QWidget *parent)
	: Field(field, parent)
{
	image = field->getImage();
}

QImage CardField::getImage()
{
	return image;
}

void CardField::setImage(const QImage &setImage)
{
	image = setImage;
}

QByteArray CardField::getBytes()
{
	return QByteArray();
}

void CardField::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);

	QPainter painter(this);

	FieldDisplayUtils::drawField(&painter, id, sidebar);

	FieldImageDisplayUtils::drawImage(&painter, id, image, sidebar);
	FieldNameDisplayUtils::drawName(&painter, name, sidebar);
}
