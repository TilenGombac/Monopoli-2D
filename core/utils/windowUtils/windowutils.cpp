#include "windowutils.h"

#include <QWidget>
#include <QApplication>
#include <QDesktopWidget>
#include <QPushButton>

#include "core/utils/constantutils.h"
#include "core/utils/boardUtils/boardutils.h"
#include "core/utils/resourceutils.h"

void WindowUtils::centerToScreen(QWidget *parent)
{
	QDesktopWidget *desktopWidget = QApplication::desktop();

	QRect screenRect = desktopWidget->availableGeometry(parent);

	int screenWidth  = screenRect.width();
	int screenHeight = screenRect.height();
	int windowWidth  = parent->width();
	int windowHeight = parent->height();

	int center_x = (screenWidth  - windowWidth) / 2.0;
	int center_y = (screenHeight - windowHeight) / 2.0;

	parent->move(center_x, center_y);
}

void WindowUtils::setFrameless(QWidget *parent)
{
	parent->setWindowFlags(Qt::FramelessWindowHint);
}

QSize WindowUtils::getWindowSize()
{
	int boardLen = BoardUtils::getBoardSize().width();

	int width  = boardLen + ConstantUtils::SIDEBAR_WIDTH;
	int height = boardLen;

	return QSize(width, height);
}

QSize WindowUtils::BUTTON_SIZE = QSize(20, 20);
