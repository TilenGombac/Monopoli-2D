#include "networkplayer.h"

#include "core/utils/playerUtils/playerutils.h"

NetworkPlayer::NetworkPlayer(Token *setToken)
	: Player(setToken, "", -1)
{
	playerType = PlayerType::NETWORK;

	id = PlayerUtils::getNetworkPlayerId();
}

NetworkPlayer::NetworkPlayer(Token *setToken, const QString &setName)
	: Player(setToken, setName, -1)
{
	playerType = PlayerType::NETWORK;

	id = PlayerUtils::getNetworkPlayerId();
}

NetworkPlayer::NetworkPlayer(Token *setToken, const QString &setName,
								const int &setId)

	: Player(setToken, setName, setId)
{
	playerType = PlayerType::NETWORK;
}
