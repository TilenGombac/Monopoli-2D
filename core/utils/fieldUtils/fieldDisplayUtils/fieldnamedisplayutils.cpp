#include "fieldnamedisplayutils.h"

#include <QPainter>

#include "core/utils/constantutils.h"
#include "core/utils/fieldUtils/fieldDisplayUtils/fielddisplayutils.h"

void FieldNameDisplayUtils::drawName(QPainter *painter, const QString &name,
									  const bool &sidebar, const bool &offset)
{
	// This method should never be called for corner fields
	int multiplier  = FieldDisplayUtils::getMultiplier(sidebar);

	int width  = ConstantUtils::FIELD_WIDTH * multiplier;
	int height = ConstantUtils::FIELD_HEIGHT * multiplier;

	// Do not display the name when very small
	if(width < 80)
		return;

	int defaultOffset = 5;

	QRect nameRect(0, defaultOffset, width, height);

	NAME_OFFSET = ConstantUtils::FIELD_HEIGHT / 4;

	if(offset)
	{
		nameRect.setY((defaultOffset + NAME_OFFSET) * multiplier);
	}

	QFont tmpFont = painter->font();

	int textWrapCenter = Qt::AlignHCenter | Qt::TextWordWrap;

	painter->setFont(FieldDisplayUtils::getFont(sidebar));
	painter->drawText(nameRect, textWrapCenter, name);
	painter->setFont(tmpFont);
}

int FieldNameDisplayUtils::NAME_OFFSET = 30;
