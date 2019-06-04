#ifndef CLIENT_H
#define CLIENT_H

#include <QTime>
#include <QTimer>
#include <QTcpSocket>
#include <QHostAddress>

#include "network/network.h"
#include "core/elements/message.h"
#include "network/packets/packet.h"

class Client : public QObject
{
	Q_OBJECT

public:
	Client(QObject *parent = nullptr);

	QTcpSocket* getSocket();
	QHostAddress getHostAddress();
	qint16 getPort();

	void calculatePing();

public slots:
	void preparePacket();
	virtual void processPacket(QByteArray data) = 0;

private slots:
	virtual void sendPingPacket() = 0;

protected:
	QTcpSocket *socket;

	QTimer *pingSendTimer;
	QTime pingSentTime;
	int ping;

signals:
	void pingChanged(int currentPing);
	void playerDisconnected(int playerId);
	void messageReceived(Message msg);

};

#endif // CLIENT_H
