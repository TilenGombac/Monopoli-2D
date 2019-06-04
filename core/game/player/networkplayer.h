#ifndef NETWORKPLAYER_H
#define NETWORKPLAYER_H

#include "core/game/player/player.h"

class NetworkPlayer : public Player
{
public:
	NetworkPlayer(Token *setToken);
	NetworkPlayer(Token *setToken, const QString &setName);
	NetworkPlayer(Token *setToken, const QString &setName, const int &setId);

};

#endif // NETWORKPLAYER_H
