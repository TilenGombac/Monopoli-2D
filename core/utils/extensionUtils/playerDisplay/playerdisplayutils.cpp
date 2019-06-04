#include "playerdisplayutils.h"

#include "core/utils/constantutils.h"

QRect PlayerDisplayUtils::getGeometry(const int &numberOfPlayers)
{
	int fieldHeight = ConstantUtils::FIELD_HEIGHT;

	int sidebarWidth = ConstantUtils::SIDEBAR_WIDTH;

	int heightPerPlayer = 50;

	int height = heightPerPlayer * numberOfPlayers;

	return QRect(0, fieldHeight, sidebarWidth, height);
}
