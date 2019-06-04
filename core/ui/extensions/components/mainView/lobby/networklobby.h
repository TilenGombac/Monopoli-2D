#ifndef NETWORKLOBBY_H
#define NETWORKLOBBY_H

#include <QHostAddress>

#include "network/packets/updateboardpacket.h"
#include "core/ui/extensions/components/mainView/lobby/lobby.h"

class Chat;
class LocalClient;
class NetworkPlayer;
class ParticipantsView;

class NetworkLobby : public Lobby
{
	Q_OBJECT

	Chat* chat;
	LocalClient* client;
	NetworkPlayer* player;
	ParticipantsView* participants;

public:
	NetworkLobby(Sidebar *sidebar, QHostAddress serverAddress,
					QWidget *parent = 0);
	~NetworkLobby();

private:
	void removeFromTokenPicker(const QString &tokenName);

public slots:
	void enablePlayerEntry();
	void addPlayer();
	void startGame();
	void onServerDown(QString reason);
	void playerConnected(NetworkPlayer* connectedPlayer);
	void playerDisconnected(int playerId);
	void setPlayerTurn(int playerId);
	void updateBoard(UpdateBoardPacket boardPacket);

};

#endif // NETWORKLOBBY_H
