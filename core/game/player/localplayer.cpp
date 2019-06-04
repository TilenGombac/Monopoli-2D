#include "localplayer.h"

LocalPlayer::LocalPlayer(Token *setToken)
	: Player(setToken)
{
	playerType = PlayerType::LOCAL;
}

LocalPlayer::LocalPlayer(Token *setToken, const QString &setName)
	: Player(setToken, setName)
{
	playerType = PlayerType::LOCAL;
}
