#include "tradepacket.h"

#include <QDataStream>

#include "core/game/player/player.h"
#include "core/game/entity/fields/possessablefield.h"

/*
 * Packet composition:
 * [Bytes - Part]
 *
 * [6 - Header][1 - TradeStatus][1 - ProviderId][1 - RecipientId]
 *		[Z - Amount][2X - Fields]
 *		[Z - Amount][2Y - Fields]
 */

TradePacket::TradePacket()
	: Packet(PacketType::TRADE)
{

}

TradePacket::TradePacket(Offer *offer, const TradeStatus &status)
	: Packet(PacketType::TRADE)
{
	int statusId = (int)status;

	QString statusIdString = QString::number(statusId);

	int providerId  = offer->getProvider()->getId();
	int recipientId = offer->getRecipient()->getId();

	float providedAmount = offer->getProvided().getAmount();
	float returnedAmount = offer->getReturned().getAmount();

	QVector<PossessableField*> providedFields = offer->getProvided().getFields();
	QVector<PossessableField*> returnedFields = offer->getReturned().getFields();

	QString providedFieldsString;
	QString returnedFieldsString;

	int fieldId;

	for(PossessableField* field : providedFields)
	{
		fieldId = field->getId();

		if(fieldId < 10)
		{
			providedFieldsString += "0";
		}

		providedFieldsString += QString::number(fieldId);
	}

	for(PossessableField* field : returnedFields)
	{
		fieldId = field->getId();

		if(fieldId < 10)
		{
			returnedFieldsString += "0";
		}

		returnedFieldsString += QString::number(fieldId);
	}

	QByteArray tmp;

	QDataStream stream(&tmp, QIODevice::WriteOnly);

	this->data.append(statusIdString);

	stream << providerId;
	stream << recipientId;

	stream << providedAmount;
	stream << returnedAmount;

	stream << providedFieldsString;
	stream << returnedFieldsString;

	this->data.append(tmp);
}

TradePacket::TradePacket(QByteArray data)
	: Packet(data)
{

}

void TradePacket::setStatus(const TradeStatus &status)
{
	int statusId = (int)status;

	QByteArray statusArray = QByteArray::number(statusId);

	this->data.replace(2, 1, statusArray);
}

TradeStatus TradePacket::getStatus()
{
	QByteArray tradeData = this->getData();

	bool ok;

	int statusId = tradeData.mid(0, 1).toInt(&ok);

	if(!ok)
		return TradeStatus::DENIED;

	TradeStatus status = (TradeStatus)statusId;

	return status;
}

int TradePacket::getProviderId()
{
	QByteArray tmp = this->getData();

	QByteArray tradeData = tmp.mid(1);

	QDataStream stream(tradeData);

	int providerId;

	stream >> providerId;

	return providerId;
}

int TradePacket::getRecipientId()
{
	QByteArray tmp = this->getData();

	QByteArray tradeData = tmp.mid(1);

	QDataStream stream(tradeData);

	int providerId;
	int recipientId;

	stream >> providerId;
	stream >> recipientId;

	return recipientId;
}

OfferPair TradePacket::getProvided(QVector<Field*> fields)
{
	QByteArray tmp = this->getData();

	QByteArray tradeData = tmp.mid(1);

	QDataStream stream(tradeData);

	OfferPair provided;

	int providerId, recipientId;

	float providedAmount, returnedAmount;

	QString providedFieldsString;

	stream >> providerId >> recipientId;

	stream >> providedAmount;

	stream >> returnedAmount;

	stream >> providedFieldsString;


	provided.changeAmount(providedAmount);

	if(providedFieldsString.size() < 2)
	{
		return provided;
	}

	QString fieldIdString;

	int fieldId;

	for(int i = 1; i < providedFieldsString.size(); i += 2)
	{
		fieldIdString = "";

		fieldIdString += providedFieldsString[i - 1];
		fieldIdString += providedFieldsString[i];

		fieldId = fieldIdString.toInt();

		for(Field* field : fields)
		{
			if(fieldId == field->getId())
			{
				if(field->getPossessable())
				{
					PossessableField* tmp = (PossessableField*)field;

					provided.addField(tmp);
				}
			}
		}
	}

	return provided;
}

OfferPair TradePacket::getReturned(QVector<Field*> fields)
{
	QByteArray tmp = this->getData();

	QByteArray tradeData = tmp.mid(1);

	QDataStream stream(tradeData);

	OfferPair returned;

	int providerId, recipientId;

	float providedAmount, returnedAmount;

	QString providedFieldsString;
	QString returnedFieldsString;

	stream >> providerId >> recipientId;

	stream >> providedAmount;

	stream >> returnedAmount;

	stream >> providedFieldsString;

	stream >> returnedFieldsString;


	returned.changeAmount(returnedAmount);

	if(returnedFieldsString.size() < 2)
	{
		return returned;
	}

	QString fieldIdString;

	int fieldId;

	for(int i = 1; i < returnedFieldsString.size(); i += 2)
	{
		fieldIdString = "";

		fieldIdString += returnedFieldsString[i - 1];
		fieldIdString += returnedFieldsString[i];

		fieldId = fieldIdString.toInt();

		for(Field* field : fields)
		{
			if(fieldId == field->getId())
			{
				if(field->getPossessable())
				{
					PossessableField* tmp = (PossessableField*)field;

					returned.addField(tmp);
				}
			}
		}
	}

	return returned;
}
