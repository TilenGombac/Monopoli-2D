#include "windowdisplayutils.h"

#include <QPainter>

#include "core/utils/windowUtils/windowutils.h"

void WindowDisplayUtils::drawBackground(QPainter *painter,
										  const QColor &color)
{
	QSize size = WindowUtils::getWindowSize();

	int width  = size.width();
	int height = size.height();

	painter->setBrush(color);
	painter->drawRect(0, 0, width, height);

	// Bottom border
	painter->drawLine(0, (height - 1), (width - 1), (height - 1));
}
