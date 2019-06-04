#ifndef WINDOWUTILS_H
#define WINDOWUTILS_H

#include <QSize>

class QWidget;

class WindowUtils
{
public:
	static QSize BUTTON_SIZE;

	static void centerToScreen(QWidget *parent);
	static void setFrameless(QWidget *parent);
	static QSize getWindowSize();

};

#endif // WINDOWUTILS_H
