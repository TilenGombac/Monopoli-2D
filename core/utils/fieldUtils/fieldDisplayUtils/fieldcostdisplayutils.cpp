#include "fieldcostdisplayutils.h"

#include <QPainter>

#include "core/utils/constantutils.h"
#include "core/utils/fieldUtils/fieldDisplayUtils/fielddisplayutils.h"

void FieldCostDisplayUtils::drawCostString(QPainter *painter,
											QString costString,
												const bool &sidebar)
{
	// This method should never be called for corner fields
	int multiplier  = FieldDisplayUtils::getMultiplier(sidebar);

	int width  = ConstantUtils::FIELD_WIDTH * multiplier;
	int height = ConstantUtils::FIELD_HEIGHT * multiplier;

	// Do not display the cost when very small
	if(width < 80)
		return;

	QFont font = FieldDisplayUtils::getFont(sidebar);

	int offset = height - font.pointSize() * 2;

	QRect priceRect(0, offset, width, height);

	QFont tmpFont = painter->font();

	int textWrapCenter = Qt::AlignHCenter | Qt::TextWordWrap;

	costString += " " + CURRENCY;

	painter->setFont(font);
	painter->drawText(priceRect, textWrapCenter, costString);
	painter->setFont(tmpFont);
}

QString FieldCostDisplayUtils::CURRENCY = "€";
