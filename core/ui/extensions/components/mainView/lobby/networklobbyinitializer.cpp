#include "networklobbyinitializer.h"

#include <QToolButton>

#include "core/elements/textline.h"
#include "core/utils/windowUtils/windowutils.h"
#include "core/utils/errorUtils/errorhandler.h"

NetworkLobbyInitializer::NetworkLobbyInitializer(QWidget *parent)
	: QWidget(parent)
{
	this->setFixedSize(300, 60);

	this->setWindowTitle("Povezava na strežnik");

	ipEdit = new TextLine(this);
	ipEdit->setPlaceholderText("Vnesi naslov strežnika...");
	ipEdit->setGeometry(0, 0, this->width(), 30);

	btn = new QToolButton(this);
	btn->setText("Pridruži se");
	btn->setGeometry(0, 30, this->width(), 30);

	this->setWindowFlags(Qt::WindowStaysOnTopHint);

	this->show();

	connect(btn, SIGNAL(clicked()), this, SLOT(attemptConnection()));
	connect(ipEdit, SIGNAL(enterPressed()), this, SLOT(attemptConnection()));
}

void NetworkLobbyInitializer::connectionAttemptFailed()
{
	this->setWindowTitle("Povezava na strežnik");

	ErrorHandler::message("Povezava ni uspela",
						"Povezava na vneseni naslov strežnika ni uspela.");

	ipEdit->setText("");
}

void NetworkLobbyInitializer::attemptConnection()
{
	QString address = ipEdit->toPlainText();

	QHostAddress addr(address);

	if(addr.protocol() == QAbstractSocket::IPv4Protocol)
	{
		this->setWindowTitle("Poskušam se povezati...");

		emit connectAttempt(addr);
	}
	else
	{
		this->connectionAttemptFailed();
	}
}
