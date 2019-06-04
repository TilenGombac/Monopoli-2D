#include "boardutils.h"

#include "core/utils/constantutils.h"

QSize BoardUtils::getBoardSize()
{
	/*
	 * Field_width = (2/3) * Field_height
	 *
	 * Board_lenth = 9 * Field_width + 2 * Field_height = 8 * Field_height
	 */

	int len = 8 * ConstantUtils::FIELD_HEIGHT;

	return QSize(len, len);
}

QRect BoardUtils::getBoardRect()
{
	QSize boardSize = getBoardSize();

	return QRect(0, 0, boardSize.width(), boardSize.height());
}
