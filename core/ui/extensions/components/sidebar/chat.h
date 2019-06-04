#ifndef CHAT_H
#define CHAT_H

#include <QWidget>

#include "core/elements/message.h"

class Client;
class TextLine;
class QTextEdit;
class QToolButton;
class BoardManager;

class Chat : public QWidget
{
	Q_OBJECT

public:
	Chat(const int &playerId, BoardManager* bm, Client* client,
			QWidget* parent = nullptr);

	void resetGeometry();

public slots:
	void addMessage(Message msg);

protected slots:
	virtual void sendMessage();

protected:
	QTextEdit* messages;
	TextLine* messageLine;
	QToolButton* sendButton;

	BoardManager* boardManager;
	Client* client;

	int playerId;

};

#endif // CHAT_H
