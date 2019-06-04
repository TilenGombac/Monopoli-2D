#include "jailpacket.h"

/*
 * Packet composition:
 * [Bytes - Part]
 *
 * [6 - Header][1 - PlayerId][1 - Action]
 */

JailPacket::JailPacket(const int &playerId, const JailAction &action)
	: Packet(PacketType::JAIL)
{
	QString playerIdString = QString::number(playerId);
	QString actionString   = QString::number((int)action);

	this->data.append(playerIdString);
	this->data.append(actionString);
}

JailPacket::JailPacket(QByteArray data)
	: Packet(data)
{

}

int JailPacket::getPlayerId()
{
	QByteArray jailData = this->getData();

	if(jailData.size() > 1)
	{
		bool ok;

		int playerId = jailData.mid(0, 1).toInt(&ok);

		if(!ok)
			return -1;

		return playerId;
	}

	return -1;
}

JailAction JailPacket::getAction()
{
	QByteArray jailData = this->getData();

	if(jailData.size() > 1)
	{
		bool ok;

		int actionValue = jailData.mid(1, 1).toInt(&ok);

		if(!ok)
			return JailAction::GO_TO_JAIL;

		JailAction action = (JailAction)actionValue;

		return action;
	}

	return JailAction::GO_TO_JAIL;
}
