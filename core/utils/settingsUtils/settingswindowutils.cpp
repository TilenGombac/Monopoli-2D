#include "settingswindowutils.h"

#include <QPainter>

#include "core/utils/windowUtils/windowutils.h"

QRect SettingsWindowUtils::getWindowGeometry()
{
	QSize windowSize = WindowUtils::getWindowSize();

	return QRect(QPoint(0, 0), windowSize);
}

void SettingsWindowUtils::drawBackground(QPainter *painter)
{
	QSize size = getWindowGeometry().size();

	QBrush brush = painter->brush();

	painter->setOpacity(0.8);

	painter->setBrush(Qt::black);

	painter->drawRect(0, 0, size.width(), size.height());

	painter->setOpacity(1);

	painter->setBrush(brush);
}
