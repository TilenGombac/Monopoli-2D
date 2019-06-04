#ifndef MESSAGE_H
#define MESSAGE_H

#include <QString>

class Message
{
	int hour;
	int minute;
	int senderId;
	QString messageContent;

public:
	Message(const int &sender, const QString &content);

	int getSenderId();
	QString getText();
	QString getMessage(const QString &senderName);

};

#endif // MESSAGE_H
