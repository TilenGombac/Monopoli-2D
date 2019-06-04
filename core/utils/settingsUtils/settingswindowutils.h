#ifndef SETTINGSWINDOWUTILS_H
#define SETTINGSWINDOWUTILS_H

#include <QRect>

class QPainter;

class SettingsWindowUtils
{
public:
	static QRect getWindowGeometry();
	static void drawBackground(QPainter *painter);

};

#endif // SETTINGSWINDOWUTILS_H
