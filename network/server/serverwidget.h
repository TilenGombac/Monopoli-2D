#ifndef SERVERWIDGET_H
#define SERVERWIDGET_H

#include <QWidget>
#include <network/server/server.h>

class ServerChat;
class ParticipantsView;

class ServerWidget : public QWidget
{
	Q_OBJECT

	ServerChat* chat;
	ParticipantsView* participantsView;

public:
	explicit ServerWidget(QWidget *parent = 0);
	~ServerWidget();

private:
	Server *server;

signals:

public slots:
};

#endif // SERVERWIDGET_H
