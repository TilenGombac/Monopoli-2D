#include "mainwindow.h"

#include <QApplication>
#include <QPainter>

#include "core/utils/windowUtils/windowutils.h"
#include "core/utils/windowUtils/windowdisplayutils.h"
#include "core/ui/extensions/mainview.h"
#include "core/ui/extensions/sidebar.h"
#include "core/utils/resourceutils.h"

#include "core/notifications/notificationmanager.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	this->setFixedSize(WindowUtils::getWindowSize());

	this->initialize();

	qApp->setStyleSheet(ResourceUtils::getStyleSheet(":/css/css/buttons/globalButton.css"));
}

MainWindow::~MainWindow()
{
}

void MainWindow::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);

	QPainter painter(this);

	WindowDisplayUtils::drawBackground(&painter, QColor(255, 255, 255));
}

void MainWindow::initialize()
{
	NotificationManager::setMainWindow(this);

	WindowUtils::setFrameless(this);
	WindowUtils::centerToScreen(this);

	this->setWindowTitle("Monopoli 2D");

	this->initializeExtensions();
}

void MainWindow::initializeExtensions()
{
	sidebar  = new Sidebar(this);
	mainView = new MainView(sidebar, this);
}

void MainWindow::setMinimized()
{
	this->setWindowState(Qt::WindowMinimized);
}

void MainWindow::quitApplication()
{
	QApplication::quit();
}
