#include "withdrawpacket.h"

/*
 * Packet composition:
 * [Bytes - Part]
 *
 * [6 - Header][1 - PlayerId][X - amount]
 */

WithdrawPacket::WithdrawPacket(const int &playerId, const float &amount)
	: Packet(PacketType::WITHDRAW)
{
	QString playerIdString = QString::number(playerId);
	QString amountString   = QString::number(amount);

	this->data.append(playerIdString);
	this->data.append(amountString);
}

WithdrawPacket::WithdrawPacket(QByteArray data)
	: Packet(data)
{

}

int WithdrawPacket::getPlayerId()
{
	QByteArray depositData = this->getData();

	if(depositData.size() > 1)
	{
		bool ok;

		int playerId = depositData.mid(0, 1).toInt(&ok);

		if(!ok)
			return -1;

		return playerId;
	}

	return -1;
}

float WithdrawPacket::getAmount()
{
	QByteArray depositData = this->getData();

	if(depositData.size() > 1)
	{
		bool ok;

		float amount = depositData.mid(1).toFloat(&ok);

		if(!ok)
			return 0;

		return amount;
	}

	return 0;
}
