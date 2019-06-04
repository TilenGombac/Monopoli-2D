#include "sidebardisplayutils.h"

#include <QPainter>

#include "core/utils/extensionUtils/sidebar/sidebarutils.h"
#include "core/utils/constantutils.h"

void SidebarDisplayUtils::drawApplicationName(QPainter *painter)
{
	int width  = ConstantUtils::SIDEBAR_WIDTH;
	int height = 60;

	painter->setFont(QFont("Verdana", 24));
	painter->drawText(20, 0, width, height, Qt::AlignVCenter, "Monopoli 2D");
}

void SidebarDisplayUtils::drawBorders(QPainter *painter)
{
	int width  = ConstantUtils::SIDEBAR_WIDTH;
	int height = SidebarUtils::getGeometry().height();

	// Top border
	painter->drawLine(0, 0, width, 0);

	// Right border
	painter->drawLine((width - 1), 0, (width - 1), height);

	// Bottom border
	painter->drawLine(0, (height - 1), width, (height - 1));

	// Left border
	painter->drawLine(0, 0, 0, (height - 1));
}
