#ifndef HOME_H
#define HOME_H

#include <QWidget>
#include <QHostAddress>

class Sidebar;
class LocalLobby;
class QPushButton;
class NetworkLobby;
class ServerWidget;
class NetworkLobbyInitializer;

class Home : public QWidget
{
	Q_OBJECT

	Sidebar *sidebar;

public:
	Home(Sidebar *setSidebar, QWidget *parent = 0);

private:
	void destroyButtons();

public slots:
	void initialize();
	void startLocalLobby();
	void startNetworkLobbyInitializer();
	void startNetworkLobby(QHostAddress serverAddress);
	void startServerWidget();

private:
	QPushButton *localLobbyButton;
	QPushButton *networkLobbyButton;
	QPushButton *serverWidgetButton;

	LocalLobby *localLobby;
	NetworkLobby *networkLobby;
	NetworkLobbyInitializer *networkLobbyInitializer;

};

#endif // HOME_H
