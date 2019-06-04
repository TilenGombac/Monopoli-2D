#include "requestpaymentpacket.h"

/*
 * Packet composition:
 * [Bytes - Part]
 *
 * [6 - Header][1 - PlayerId][X - Amount]
 */

RequestPaymentPacket::RequestPaymentPacket(const int &playerId,
											const float &amount)
	: Packet(PacketType::REQUEST_PAYMENT)
{
	QString playerIdString = QString::number(playerId);
	QString amountString   = QString::number(amount);

	this->data.append(playerIdString);
	this->data.append(amountString);
}

RequestPaymentPacket::RequestPaymentPacket(QByteArray data)
	: Packet(data)
{

}

int RequestPaymentPacket::getPlayerId()
{
	QByteArray paymentData = this->getData();

	if(paymentData.size() > 1)
	{
		bool ok;

		int playerId = paymentData.mid(0, 1).toInt(&ok);

		if(!ok)
			return -1;

		return playerId;
	}

	return -1;
}

float RequestPaymentPacket::getAmount()
{
	QByteArray paymentData = this->getData();

	if(paymentData.size() > 1)
	{
		bool ok;

		float amount = paymentData.mid(1).toFloat(&ok);

		if(!ok)
			return 0;

		return amount;
	}

	return 0;
}
