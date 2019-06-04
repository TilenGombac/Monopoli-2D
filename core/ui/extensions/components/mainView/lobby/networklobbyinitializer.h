#ifndef NETWORKLOBBYINITIALIZER_H
#define NETWORKLOBBYINITIALIZER_H

#include <QWidget>
#include <QHostAddress>

class TextLine;
class QToolButton;

class NetworkLobbyInitializer : public QWidget
{
	Q_OBJECT

	TextLine *ipEdit;
	QToolButton *btn;

public:
	explicit NetworkLobbyInitializer(QWidget *parent = 0);

	void connectionAttemptFailed();

signals:
	void connectAttempt(QHostAddress address);

public slots:
	void attemptConnection();

};

#endif // NETWORKLOBBYINITIALIZER_H
