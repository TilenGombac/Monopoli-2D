#include "home.h"

#include <QFile>
#include <QLabel>
#include <QTimer>
#include <QPushButton>

#include "core/utils/constantutils.h"
#include "core/sounds/soundmanager.h"
#include "network/server/serverwidget.h"
#include "core/ui/extensions/components/mainView/lobby/locallobby.h"
#include "core/ui/extensions/components/mainView/lobby/networklobby.h"
#include "core/ui/extensions/components/mainView/lobby/networklobbyinitializer.h"

Home::Home(Sidebar *setSidebar, QWidget *parent)
	: QWidget(parent)
{
	sidebar = setSidebar;

	localLobby   = nullptr;
	networkLobby = nullptr;

	this->initialize();
}

void Home::destroyButtons()
{
	localLobbyButton->deleteLater();

	networkLobbyButton->deleteLater();

	serverWidgetButton->deleteLater();
}

void Home::initialize()
{
	SoundManager::startAmbient(Sound::HOME);

	this->show();

	this->setGeometry(this->parentWidget()->geometry());

	localLobbyButton   = new QPushButton(this);
	networkLobbyButton = new QPushButton(this);
	serverWidgetButton = new QPushButton(this);

	int btnWidth = ConstantUtils::FIELD_WIDTH * 3.5;

	localLobbyButton->setText("Začni lokalno igro");
	localLobbyButton->setGeometry(10, 10, btnWidth, 30);

	networkLobbyButton->setText("Začni LAN igro");
	networkLobbyButton->setGeometry((btnWidth + 20), 10, btnWidth, 30);

	serverWidgetButton->setText("Zaženi strežnik");
	serverWidgetButton->setGeometry((btnWidth + 15) * 2, 10, btnWidth, 30);

	localLobbyButton->show();
	networkLobbyButton->show();
	serverWidgetButton->show();

	connect(localLobbyButton, SIGNAL(clicked()),
				this, SLOT(startLocalLobby()));

	connect(networkLobbyButton, SIGNAL(clicked()),
				this, SLOT(startNetworkLobbyInitializer()));

	connect(serverWidgetButton, SIGNAL(clicked()),
				this, SLOT(startServerWidget()));

	QFile highscoreFile("highscore.txt");
	if(highscoreFile.open(QIODevice::ReadOnly))
	{
		QString highscoreText = "Najboljši rezultat\n"
					+ highscoreFile.readAll();

		QLabel* highscore = new QLabel(highscoreText, this);
		highscore->setGeometry(20, this->height() - 100, this->width() - 20,
								100);

		QFont font = highscore->font();
		font.setPointSize(18);

		highscore->setFont(font);
	}
}

void Home::startLocalLobby()
{
	localLobby = new LocalLobby(sidebar, this);
	localLobby->setGeometry(this->geometry());
	localLobby->show();

	this->destroyButtons();
}

void Home::startNetworkLobbyInitializer()
{
	networkLobbyInitializer = new NetworkLobbyInitializer();

	connect(networkLobbyInitializer, SIGNAL(connectAttempt(QHostAddress)),
				this, SLOT(startNetworkLobby(QHostAddress)));
}

void Home::startNetworkLobby(QHostAddress serverAddress)
{
	QTcpSocket socket;
	socket.connectToHost(serverAddress, SERVER_PORT);
	if(socket.waitForConnected(5000))
	{
		networkLobby = new NetworkLobby(sidebar, serverAddress, this);
		networkLobby->setGeometry(this->geometry());
		networkLobby->show();

		networkLobbyInitializer->deleteLater();

		this->destroyButtons();
	}
	else
	{
		networkLobbyInitializer->connectionAttemptFailed();
	}
}

void Home::startServerWidget()
{
	ServerWidget* w = new ServerWidget(0);
	connect(this, SIGNAL(destroyed(QObject*)),
				w, SLOT(deleteLater()));
	w->show();
}
