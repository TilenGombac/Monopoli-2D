#include "fieldgeometryutils.h"

#include "core/utils/constantutils.h"
#include "core/utils/boardUtils/boardutils.h"

QRect FieldGeometryUtils::getRect(const int &fieldId)
{
	QPoint topLeft = getPosition(fieldId);

	QSize fieldSize = getSize(fieldId);

	return QRect(topLeft, fieldSize);
}

QRect FieldGeometryUtils::getRotatedRect(const int &fieldId)
{
	QPoint topLeft = getPosition(fieldId);

	QSize fieldSize = getRotatedSize(fieldId);

	return QRect(topLeft, fieldSize);
}

QSize FieldGeometryUtils::getSize(const int &fieldId)
{
	int fieldWidth  = ConstantUtils::FIELD_WIDTH;
	int fieldHeight = ConstantUtils::FIELD_HEIGHT;

	// Corner fields [0, 10, 20, 30]
	if(fieldId % 10 == 0)
	{
		return QSize(fieldHeight, fieldHeight);
	}

	// Other fields
	return QSize(fieldWidth, fieldHeight);
}

QSize FieldGeometryUtils::getRotatedSize(const int &fieldId)
{
	int fieldWidth  = ConstantUtils::FIELD_WIDTH;
	int fieldHeight = ConstantUtils::FIELD_HEIGHT;

	// Left and right rows [11, 19] & [31, 39]
	//	+ extra condition (% 10) so corner fields get excluded
	if((fieldId / 10 == 1 || fieldId / 10 == 3) && fieldId % 10 != 0)
	{
		return QSize(fieldHeight, fieldWidth);
	}

	// Other fields are not impacted
	return getSize(fieldId);
}

QPoint FieldGeometryUtils::getPosition(const int &fieldId)
{
	int x = getPositionX(fieldId);
	int y = getPositionY(fieldId);

	return QPoint(x, y);
}

int FieldGeometryUtils::getPositionX(const int &fieldId)
{
	int fieldWidth  = ConstantUtils::FIELD_WIDTH;
	int fieldHeight = ConstantUtils::FIELD_HEIGHT;

	// 0 - 9
	if(fieldId / 10 == 0)
	{
		return fieldHeight + (9 - fieldId) * fieldWidth;
	}

	// 10 - 20
	if(fieldId >= 10 && fieldId <= 20)
	{
		return 0;
	}

	// 21 - 30
	if((fieldId - 1) / 10 == 2)
	{
		return fieldHeight + (fieldId - 21) * fieldWidth;
	}

	// 31 - 39
	int boardWidth = BoardUtils::getBoardSize().width();

	return boardWidth - fieldHeight;
}

int FieldGeometryUtils::getPositionY(const int &fieldId)
{
	int fieldWidth  = ConstantUtils::FIELD_WIDTH;
	int fieldHeight = ConstantUtils::FIELD_HEIGHT;

	int boardHeight = BoardUtils::getBoardSize().height();

	// 0 - 9
	if(fieldId / 10 == 0)
	{
		return boardHeight - fieldHeight;
	}

	// 10 - 19
	if(fieldId / 10 == 1)
	{
		return fieldHeight + (19 - fieldId) * fieldWidth;
	}

	// 20 - 30
	if(fieldId >= 20 && fieldId <= 30)
	{
		return 0;
	}

	// 31 - 39
	return fieldHeight + (fieldId - 31) * fieldWidth;
}
