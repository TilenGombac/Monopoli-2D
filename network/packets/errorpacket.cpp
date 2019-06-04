#include "errorpacket.h"

/*
 * Packet composition:
 * [Bytes - Part]
 *
 * [6 - Header][2 - ErrorCode][X - ErrorMessage]
 */

ErrorPacket::ErrorPacket(const ErrorCode &code, const QString &errorMessage)
	: Packet(PacketType::ERROR)
{
	QString errorCodeString;

	if((int)code < 10)
		errorCodeString += "0";

	errorCodeString += QString::number((int)code);

	this->data.append(errorCodeString);
	this->data.append(errorMessage);
}

ErrorPacket::ErrorPacket(QByteArray data)
	: Packet(data)
{

}

ErrorCode ErrorPacket::getErrorCode()
{
	QByteArray errorData = this->getData();

	if(errorData.size() > 1)
	{
		bool ok;

		int errorCode = errorData.mid(0, 2).toInt(&ok);

		if(!ok)
			return ErrorCode::UNKNOWN;

		return (ErrorCode)errorCode;
	}

	return ErrorCode::UNKNOWN;
}

QString ErrorPacket::getErrorMessage()
{
	QByteArray errorData = this->getData();

	if(errorData.size() > 2)
	{
		QString errorMessage = errorData.mid(2);

		return errorMessage;
	}

	return "";
}
