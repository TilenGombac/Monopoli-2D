#include "movepacket.h"

/*
 * Packet composition:
 * [Bytes - Part]
 *
 * [6 - Header][1 - PlayerId][2 - FieldId]
 */

MovePacket::MovePacket(const int &playerId, const int &fieldId)
	: Packet(PacketType::MOVE)
{
	QString playerIdString = QString::number(playerId);

	QString fieldIdString;

	if(fieldId < 10)
	{
		fieldIdString += "0";
	}

	fieldIdString += QString::number(fieldId);

	this->data.append(playerIdString);
	this->data.append(fieldIdString);
}

MovePacket::MovePacket(QByteArray data)
	: Packet(data)
{

}

int MovePacket::getPlayerId()
{
	QByteArray moveData = this->getData();

	if(moveData.size() > 1)
	{
		bool ok;

		int playerId = moveData.mid(0, 1).toInt(&ok);

		if(!ok)
			return -1;

		return playerId;
	}

	return -1;
}

int MovePacket::getFieldId()
{
	QByteArray moveData = this->getData();

	if(moveData.size() > 1)
	{
		bool ok;

		int fieldId = moveData.mid(1, 2).toInt(&ok);

		if(!ok)
			return -1;

		return fieldId;
	}

	return -1;
}
