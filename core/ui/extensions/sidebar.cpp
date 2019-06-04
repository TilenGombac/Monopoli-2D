#include "sidebar.h"

#include <QPainter>

#include "core/utils/constantutils.h"
#include "core/utils/windowUtils/windowutils.h"
#include "core/utils/boardUtils/boardutils.h"
#include "core/utils/extensionUtils/sidebar/sidebarutils.h"
#include "core/utils/extensionUtils/sidebar/sidebardisplayutils.h"
#include "core/ui/extensions/components/sidebar/controlbuttons.h"
#include "core/game/board/board.h"

Sidebar::Sidebar(QWidget *parent)
	: DraggableWidget(parent)
{

	initialize();
}

Sidebar::~Sidebar()
{

}

void Sidebar::paintEvent(QPaintEvent *event)
{	
	Q_UNUSED(event);

	QPainter painter(this);

	SidebarDisplayUtils::drawApplicationName(&painter);
	SidebarDisplayUtils::drawBorders(&painter);
}

void Sidebar::initialize()
{
	this->setGeometry(SidebarUtils::getGeometry());


	// Components

	// parent because of slots
	controlButtons = new ControlButtons(this->parentWidget());
}
