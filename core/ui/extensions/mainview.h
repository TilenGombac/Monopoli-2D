#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QWidget>

class Home;
class Lobby;
class Player;
class Sidebar;

class MainView : public QWidget
{
	Q_OBJECT

	Sidebar *sidebar;

	// Components
	Home *home;
	Lobby *lobby;

public:
	MainView(Sidebar *setSidebar, QWidget *parent = 0);
	~MainView();

	Home* getHome();
	Sidebar* getSidebar();

public slots:
	void endGameScreen(Player* winner);

private:
	void initializeComponents();

};

#endif // MAINVIEW_H
