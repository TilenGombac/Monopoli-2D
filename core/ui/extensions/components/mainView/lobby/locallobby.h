#ifndef LOCALLOBBY_H
#define LOCALLOBBY_H

#include "core/ui/extensions/components/mainView/lobby/lobby.h"

class SinglePlayerGame;

class LocalLobby : public Lobby
{
	Q_OBJECT

public:
	LocalLobby(Sidebar *sidebar, QWidget *parent = 0);
	~LocalLobby();

	void initializeGeometry();

public slots:
	void addPlayer();
	void startGame();
	void updateName();

private:
	SinglePlayerGame *game;

};

#endif // LOCALLOBBY_H
