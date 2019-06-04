#include "playerutils.h"

int PlayerUtils::getPlayerId()
{
	AVAILABLE_PLAYER_ID++;

	return AVAILABLE_PLAYER_ID;
}

int PlayerUtils::getNetworkPlayerId()
{
	AVAILABLE_NETWORK_PLAYER_ID++;

	return AVAILABLE_NETWORK_PLAYER_ID;
}

void PlayerUtils::resetPlayerId()
{
	// -1 is the reset (and default) value
	AVAILABLE_PLAYER_ID = -1;
}

void PlayerUtils::resetNetworkPlayerId()
{
	AVAILABLE_NETWORK_PLAYER_ID = -1;
}

QColor PlayerUtils::getPlayerColor(const int &playerId)
{
	if(playerId == 0)
		return QColor(239, 111, 108);
	else if(playerId == 1)
		return QColor(70, 87, 117);
	else if(playerId == 2)
		return QColor(10, 185, 76);
	else if(playerId == 3)
		return QColor(130, 9, 51);
	else if(playerId == 4)
		return QColor(194, 9, 90);
	else if(playerId == 5)
		return QColor(211, 84, 0);

	else
		return QColor();
}

int PlayerUtils::AVAILABLE_PLAYER_ID         = -1;
int PlayerUtils::AVAILABLE_NETWORK_PLAYER_ID = -1;
