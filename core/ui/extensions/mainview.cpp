#include "mainview.h"

#include "core/game/player/player.h"
#include "core/game/victoryscreen.h"
#include "core/ui/extensions/sidebar.h"
#include "core/utils/boardUtils/boardutils.h"
#include "core/ui/extensions/components/mainView/home.h"

MainView::MainView(Sidebar *setSidebar, QWidget *parent)
	: QWidget(parent)
{
	sidebar = setSidebar;

	this->setGeometry(BoardUtils::getBoardRect());

	this->initializeComponents();

	this->show();
}

MainView::~MainView()
{
}

Home* MainView::getHome()
{
	return home;
}

Sidebar* MainView::getSidebar()
{
	return sidebar;
}

void MainView::endGameScreen(Player *winner)
{
	QString winnerName = winner->getName();

	int netWorth = winner->getNetWorth();

	this->parentWidget()->hide();

	VictoryScreen* victoryScreen = new VictoryScreen(winnerName, netWorth);

	victoryScreen->show();
}

void MainView::initializeComponents()
{
	home = new Home(sidebar, this);
}
