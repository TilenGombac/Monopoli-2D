#include "playeractionpacket.h"

/*
 * Packet composition:
 * [Bytes - Part]
 *
 * [6 - Header][1 - PlayerId][2 - FieldId][1 - MP_PlayerAction]
 */

PlayerActionPacket::PlayerActionPacket(const int &playerId,
										const int &fieldId,
											const MP_PlayerAction &action)
	: Packet(PacketType::PLAYER_ACTION)
{
	QString playerIdString     = QString::number(playerId);
	QString playerActionString = QString::number((int)action);

	QString fieldIdString;

	if(fieldId < 10)
	{
		fieldIdString = "0";
	}

	fieldIdString += QString::number(fieldId);

	this->data.append(playerIdString);
	this->data.append(fieldIdString);
	this->data.append(playerActionString);
}

PlayerActionPacket::PlayerActionPacket(QByteArray data)
	: Packet(data)
{

}

int PlayerActionPacket::getPlayerId()
{
	QByteArray actionData = this->getData();

	if(actionData.size() > 2)
	{
		bool ok;

		int playerId = actionData.mid(0, 1).toInt(&ok);

		if(!ok)
			return -1;

		return playerId;
	}

	return -1;
}

int PlayerActionPacket::getFieldId()
{
	QByteArray actionData = this->getData();

	if(actionData.size() > 2)
	{
		bool ok;

		int fieldId = actionData.mid(1, 2).toInt(&ok);

		if(!ok)
			return -1;

		return fieldId;
	}

	return -1;
}

MP_PlayerAction PlayerActionPacket::getPlayerAction()
{
	QByteArray actionData = this->getData();

	if(actionData.size() > 2)
	{
		bool ok;

		int actionValue = actionData.mid(3, 1).toInt(&ok);

		if(!ok)
			return MP_PlayerAction::NONE;

		MP_PlayerAction action = (MP_PlayerAction)actionValue;

		return action;
	}

	return MP_PlayerAction::NONE;
}
