#include "playerdashboardutils.h"

#include "core/utils/constantutils.h"
#include "core/utils/extensionUtils/fieldDisplay/sidebarfieldutils.h"

QRect PlayerDashboardUtils::getDashboardGeometry()
{
	int x = 0;
	int y = SidebarFieldUtils::getGeometry().y() - DASHBOARD_HEIGHT;

	return QRect(x, y, ConstantUtils::SIDEBAR_WIDTH, DASHBOARD_HEIGHT);
}

int PlayerDashboardUtils::DASHBOARD_HEIGHT = 100;
