#include "gamecontrollerutils.h"

#include "core/utils/constantutils.h"
#include "core/utils/windowUtils/windowutils.h"

QRect GameControllerUtils::getGeometry()
{
	int width  = ConstantUtils::SIDEBAR_WIDTH;
	int height = WindowUtils::getWindowSize().height();

	return QRect(0, 0, width, height);
}
