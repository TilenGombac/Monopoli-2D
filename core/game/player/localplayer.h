#ifndef LOCALPLAYER_H
#define LOCALPLAYER_H

#include "core/game/player/player.h"

class LocalPlayer : public Player
{
public:
	LocalPlayer(Token *setToken);
	LocalPlayer(Token *setToken, const QString &setName);
};

#endif // LOCALPLAYER_H
