#include "fieldtranslateutils.h"

#include "core/utils/constantutils.h"
#include "core/utils/fieldUtils/fieldrotationutils.h"

QPoint FieldTranslateUtils::getTranslation(const int &fieldId)
{
	int fieldWidth  = ConstantUtils::FIELD_WIDTH;
	int fieldHeight = ConstantUtils::FIELD_HEIGHT;

	// Corner fields
	if(fieldId % 10 == 0)
		fieldWidth = ConstantUtils::FIELD_HEIGHT;

	int rotation = FieldRotationUtils::getRotation(fieldId);

	// top left corner moved by fieldHeight to the right
	if(rotation == 90)
	{
		return QPoint(fieldHeight, 0);
	}

	// top left corner moved by fieldWidth to the right
	//	and by fieldHeight downward
	if(rotation == 180)
	{
		return QPoint(fieldWidth, fieldHeight);
	}

	// top left corner moved by fieldWidth downward
	if(rotation == 270)
	{
		return QPoint(0, fieldWidth);
	}

	// default - no translation
	return QPoint(0, 0);
}
