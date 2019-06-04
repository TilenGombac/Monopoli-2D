#include "connectpacket.h"

#include "core/game/entity/token.h"
#include "core/game/player/networkplayer.h"
#include "core/game/entity/tokens/automobiletoken.h"
#include "core/game/entity/tokens/battleshiptoken.h"
#include "core/game/entity/tokens/howitzertoken.h"
#include "core/game/entity/tokens/scottishterriertoken.h"
#include "core/game/entity/tokens/thimbletoken.h"
#include "core/game/entity/tokens/tophattoken.h"
#include "core/game/entity/fields/cornerfield.h"

/*
 * Packet composition:
 * [Bytes - Part]
 *
 * [6 - Header][1 - TokenType][1 - PlayerId][X - PlayerName]
 */

ConnectPacket::ConnectPacket(NetworkPlayer *player)
	: Packet(PacketType::CONNECT)
{
	if(player != nullptr)
	{
		// Connect -> mostly default values so no need to send them ;)
		QString playerName  = player->getName();
		TokenType tokenType = player->getToken()->getTokenType();

		int playerId = player->getId();

		QString tokenTypeString = QString::number((int)tokenType);

		QString playerIdString  = QString::number((int)playerId);

		this->data.append(tokenTypeString);
		this->data.append(playerIdString);
		this->data.append(playerName);
	}
}

ConnectPacket::ConnectPacket(QByteArray data)
	: Packet(data)
{

}

NetworkPlayer* ConnectPacket::getPlayer()
{
	// 9 is the uninitialized player id

	QByteArray playerData = this->getData();

	if(playerData.size() > 1)
	{
		bool ok;
		TokenType tokenType =
				(TokenType)playerData.mid(0, 1).toInt(&ok);

		if(!ok)
			return nullptr;

		int playerId = playerData.mid(1, 1).toInt(&ok);

		if(!ok)
			return nullptr;

		QString playerName  = playerData.mid(2);

		Token *token;

		if(tokenType == TokenType::AUTOMOBILE)
			token = new AutomobileToken(new CornerField(0));

		else if(tokenType == TokenType::BATTLESHIP)
			token = new BattleshipToken(new CornerField(0));

		else if(tokenType == TokenType::HOWITZER)
			token = new HowitzerToken(new CornerField(0));

		else if(tokenType == TokenType::SCOTTISH_TERRIER)
			token = new ScottishTerrierToken(new CornerField(0));

		else if(tokenType == TokenType::THIMBLE)
			token = new ThimbleToken(new CornerField(0));

		else
			token = new TopHatToken(new CornerField(0));

		NetworkPlayer *player =
				new NetworkPlayer(token, playerName, playerId);

		return player;
	}

	return nullptr;
}
