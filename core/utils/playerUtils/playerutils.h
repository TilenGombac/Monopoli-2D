#ifndef PLAYERUTILS_H
#define PLAYERUTILS_H

#include <QColor>

class PlayerUtils
{
	static int AVAILABLE_PLAYER_ID;
	static int AVAILABLE_NETWORK_PLAYER_ID;

public:
	// Returns the next available player id
	static int getPlayerId();

	static int getNetworkPlayerId();

	static void resetPlayerId();
	static void resetNetworkPlayerId();

	static QColor getPlayerColor(const int &playerId);

};

#endif // PLAYERUTILS_H
