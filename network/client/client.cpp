#include "client.h"

#include "network/packets/pingpacket.h"

Client::Client(QObject *parent)
	: QObject(parent)
{
	pingSendTimer = new QTimer(this);
	pingSendTimer->start(1000);

	connect(pingSendTimer, SIGNAL(timeout()), this, SLOT(sendPingPacket()));
}

QTcpSocket* Client::getSocket()
{
	return socket;
}

QHostAddress Client::getHostAddress()
{
	return socket->peerAddress();
}

qint16 Client::getPort()
{
	return socket->peerPort();
}

void Client::calculatePing()
{
	int currentPing = pingSentTime.elapsed();

	if(currentPing != ping)
	{
		ping = currentPing;

		emit pingChanged(ping);
	}

	pingSendTimer->start(1000);
}

void Client::preparePacket()
{
	while(this->socket->bytesAvailable() > 0)
	{
		QByteArray data;

		int dataSize;

		this->socket->read((char*)&dataSize, sizeof(int));

		data = this->socket->read(dataSize);

		// The whole packet was not received, wait for the rest
		while(data.size() < dataSize)
		{
			this->socket->waitForReadyRead();
			data.append(this->socket->read(dataSize - data.size()));
		}

		this->processPacket(data);
	}
}
