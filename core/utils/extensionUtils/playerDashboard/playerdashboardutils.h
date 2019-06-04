#ifndef PLAYERDASHBOARDUTILS_H
#define PLAYERDASHBOARDUTILS_H

#include <QRect>

class PlayerDashboardUtils
{
public:
	static int DASHBOARD_WIDTH;
	static int DASHBOARD_HEIGHT;

	static QRect getDashboardGeometry();

};

#endif // PLAYERDASHBOARDUTILS_H
