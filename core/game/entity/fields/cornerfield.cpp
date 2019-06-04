#include "cornerfield.h"

#include <QPainter>

#include "core/utils/fieldUtils/fieldDisplayUtils/fielddisplayutils.h"
#include "core/utils/fieldUtils/fieldDisplayUtils/fieldimagedisplayutils.h"

CornerField::CornerField(QWidget *parent)
	: Field(parent)
{
	fieldType = FieldType::CORNER_FIELD;
}

CornerField::CornerField(const int &id, QWidget *parent)
	: Field(id, parent)
{
	fieldType = FieldType::CORNER_FIELD;
}

CornerField::CornerField(CornerField *field, QWidget *parent)
	: Field(field, parent)
{
	image = field->getImage();
}

QImage CornerField::getImage()
{
	return image;
}

void CornerField::setImage(const QImage &setImage)
{
	image = setImage;
}

QByteArray CornerField::getBytes()
{
	return QByteArray();
}

void CornerField::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);

	QPainter painter(this);

	FieldDisplayUtils::drawField(&painter, id, sidebar);

	FieldImageDisplayUtils::drawImage(&painter, id, image, sidebar);
}
