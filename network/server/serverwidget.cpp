#include "serverwidget.h"

#include <QLabel>

#include "core/utils/constantutils.h"
#include "network/server/serverchat.h"
#include "core/utils/playerUtils/playerutils.h"
#include "core/ui/extensions/components/sidebar/participantsview.h"

ServerWidget::ServerWidget(QWidget *parent)
	: QWidget(parent, Qt::Window)
{
	this->setFixedSize(ConstantUtils::SIDEBAR_WIDTH, 600);

	server = new Server(this);

	QHostAddress address = server->getAddress();

	QLabel *runningInfo = new QLabel(this);
	runningInfo->setText("Strežnik teče na naslovu: " + address.toString());
	runningInfo->setGeometry(10, 10, this->width(), 30);

	this->setWindowTitle("Monopoli2D Strežnik");

	this->setAttribute(Qt::WA_DeleteOnClose);

	participantsView = new ParticipantsView(true, this);
	participantsView->setGeometry(0, 50, participantsView->width(),
									participantsView->height());

	connect(server, SIGNAL(playerAdded(Player*)),
				participantsView, SLOT(addParticipant(Player*)));

	connect(server, SIGNAL(playerRemoved(Player*)),
				participantsView, SLOT(removeParticipant(Player*)));

	this->chat = server->getChat();
	this->chat->setParent(this);
	this->chat->resetGeometry();

	server->setParticipantsView(participantsView);
}

ServerWidget::~ServerWidget()
{
	server->deleteLater();

	PlayerUtils::resetNetworkPlayerId();
}
