#include "fieldcolordisplayutils.h"

#include <QPainter>

#include "core/utils/constantutils.h"
#include "core/utils/fieldUtils/fieldDisplayUtils/fielddisplayutils.h"

void FieldColorDisplayUtils::drawColorBox(QPainter *painter,
											const QColor &color,
												const bool &sidebar)
{
	// This method should never be called for corner fields
	int multiplier  = FieldDisplayUtils::getMultiplier(sidebar);

	int width  = ConstantUtils::FIELD_WIDTH * multiplier;
	int height = ConstantUtils::FIELD_HEIGHT * multiplier / 4;

	QBrush tmpBrush = painter->brush();

	QRect colorRect(0, 0, width, height);

	painter->setBrush(QBrush(color));
	painter->drawRect(colorRect);
	painter->setBrush(tmpBrush);
}
