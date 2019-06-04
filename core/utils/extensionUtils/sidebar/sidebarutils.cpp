#include "sidebarutils.h"

#include "core/utils/boardUtils/boardutils.h"
#include "core/utils/constantutils.h"
#include "core/utils/windowUtils/windowutils.h"

QRect SidebarUtils::getGeometry()
{
	int boardWidth    = BoardUtils::getBoardSize().width();
	int sidebarWidth  = ConstantUtils::SIDEBAR_WIDTH;
	int sidebarHeight = WindowUtils::getWindowSize().height();

	return QRect(boardWidth, 0, sidebarWidth, sidebarHeight);
}
