#include "fielddisplayutils.h"

#include <QPainter>

#include "core/utils/constantutils.h"
#include "core/utils/fieldUtils/fieldgeometryutils.h"
#include "core/utils/fieldUtils/fieldtranslateutils.h"
#include "core/utils/fieldUtils/fieldrotationutils.h"

void FieldDisplayUtils::drawField(QPainter *painter, const int &fieldId,
									const bool &sidebar)
{
	// Don't want rotation in the sidebar
	if(!sidebar)
	{
		transform(painter, fieldId);
	}

	drawFieldRect(painter, fieldId, sidebar);
}

QFont FieldDisplayUtils::getFont(const bool &sidebar)
{
	int fieldWidth  = ConstantUtils::FIELD_WIDTH;

	int pointSize = fieldWidth / 8;

	if(sidebar)
	{
		pointSize = fieldWidth / 4;
	}

	QFont font("Verdana");

	font.setPointSize(pointSize);

	return font;
}

QSize FieldDisplayUtils::getSize(const int &fieldId, const bool &sidebar)
{
	QSize fieldSize = FieldGeometryUtils::getSize(fieldId);

	int width  = fieldSize.width() * getMultiplier(sidebar);
	int height = fieldSize.height() * getMultiplier(sidebar);

	return QSize(width, height);
}

int FieldDisplayUtils::getMultiplier(const bool &sidebar)
{
	int multiplier = 1;

	if(sidebar)
	{
		multiplier = 2;
	}

	return multiplier;
}

void FieldDisplayUtils::transform(QPainter *painter, const int &fieldId)
{
	painter->translate(FieldTranslateUtils::getTranslation(fieldId));
	painter->rotate(FieldRotationUtils::getRotation(fieldId));
}

void FieldDisplayUtils::drawFieldRect(QPainter *painter, const int &fieldId,
										const bool &sidebar)
{
	QSize size = getSize(fieldId, sidebar);

	// 0,0 because the translation already takes care of the position
	painter->drawRect(0, 0, size.width(), size.height());
}
