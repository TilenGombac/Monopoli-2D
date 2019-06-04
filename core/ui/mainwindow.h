#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Sidebar;
class MainView;

class MainWindow : public QMainWindow
{
	Q_OBJECT

	// Extensions
	Sidebar *sidebar;
	MainView *mainView;

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

protected:
	void paintEvent(QPaintEvent *event);

private slots:
	void initialize();
	void initializeExtensions();

public slots:
	void setMinimized();
	void quitApplication();

};

#endif // MAINWINDOW_H
