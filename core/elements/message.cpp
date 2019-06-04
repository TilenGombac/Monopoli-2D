#include "message.h"

#include <QTime>

Message::Message(const int &sender, const QString &content)
{
	this->senderId       = sender;
	this->messageContent = content;

	QTime currentTime = QTime::currentTime();

	this->hour   = currentTime.hour();
	this->minute = currentTime.minute();
}

int Message::getSenderId()
{
	return this->senderId;
}

QString Message::getText()
{
	return this->messageContent;
}

QString Message::getMessage(const QString &senderName)
{
	QString message;
	QString hourString;
	QString minuteString;

	if(hour < 10)
	{
		hourString += "0";
	}

	if(minute < 10)
	{
		minuteString += "0";
	}

	hourString   += QString::number(hour);
	minuteString += QString::number(minute);

	// Format: [HH:mm] Sender: content

	message = "[" + hourString + ":" + minuteString + "] ";

	message += senderName + ": ";

	message += messageContent + '\n';

	return message;
}
