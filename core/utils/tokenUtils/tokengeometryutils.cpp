#include "tokengeometryutils.h"

#include "core/utils/fieldUtils/fieldgeometryutils.h"

QRect TokenGeometryUtils::getRect(const int &fieldId, const QPixmap &pixmap)
{
	QSize size   = getSize(pixmap);
	QPoint point = getPosition(fieldId, pixmap);

	return QRect(point, size);
}

QSize TokenGeometryUtils::getSize(const QPixmap &pixmap)
{
	return pixmap.size();
}

QPoint TokenGeometryUtils::getPosition(const int fieldId,
										const QPixmap &pixmap)
{
	int x = getPositionX(fieldId, pixmap);
	int y = getPositionY(fieldId, pixmap);

	return QPoint(x, y);
}

int TokenGeometryUtils::getPositionX(const int fieldId,
										const QPixmap &pixmap)
{
	int tokenWidth  = pixmap.width();

	QRect fieldRect = FieldGeometryUtils::getRect(fieldId);

	int fieldX = fieldRect.x();

	int fieldWidth  = fieldRect.width();

	// Left and right rows, but not corner fields
	if(((fieldId / 10 == 1) || (fieldId / 10 == 3)) && (fieldId % 10 != 0))
	{
		fieldWidth = fieldRect.height();
	}

	return fieldX + (fieldWidth - tokenWidth) / 2;
}

int TokenGeometryUtils::getPositionY(const int fieldId,
										const QPixmap &pixmap)
{
	int tokenHeight = pixmap.height();

	QRect fieldRect = FieldGeometryUtils::getRect(fieldId);

	int fieldY = fieldRect.y();

	int fieldHeight = fieldRect.height();

	// Left and right rows, but not corner fields
	if(((fieldId / 10 == 1) || (fieldId / 10 == 3)) && (fieldId % 10 != 0))
	{
		fieldHeight = fieldRect.width();
	}

	return fieldY + (fieldHeight - tokenHeight) / 2;
}
