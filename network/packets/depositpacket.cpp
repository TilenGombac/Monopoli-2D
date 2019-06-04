#include "depositpacket.h"

/*
 * Packet composition:
 * [Bytes - Part]
 *
 * [6 - Header][1 - PlayerId][X - amount]
 */

DepositPacket::DepositPacket(const int &playerId, const float &amount)
	: Packet(PacketType::DEPOSIT)
{
	QString playerIdString = QString::number(playerId);
	QString amountString   = QString::number(amount);

	this->data.append(playerIdString);
	this->data.append(amountString);
}

DepositPacket::DepositPacket(QByteArray data)
	: Packet(data)
{

}

int DepositPacket::getPlayerId()
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

float DepositPacket::getAmount()
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
