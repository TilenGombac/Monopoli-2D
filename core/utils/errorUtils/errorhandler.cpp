#include "errorhandler.h"

#include <QFile>
#include <QDateTime>
#include <QTextStream>
#include <QMessageBox>

#include "core/notifications/notificationmanager.h"

void ErrorHandler::message(const QString &title, const QString &text)
{
	/*
	QMessageBox *messageBox = new QMessageBox();

	messageBox->setWindowTitle(title);
	messageBox->setText(text);
	messageBox->setWindowFlags(Qt::WindowStaysOnTopHint);
	messageBox->setAttribute(Qt::WA_DeleteOnClose);
	messageBox->show();
	*/

	NotificationManager::pushNotification(title, text);

	log(title, text);
}

void ErrorHandler::log(const QString &title, const QString &text)
{
	QDateTime current = QDateTime::currentDateTime();
	QString format = current.toString("dd'-'MM'-'yyyy");

	QString filename = "log_" + format + ".txt";

	QFile file(filename);

	QString timestamp = current.toString("'['hh':'mm':'ss'] '");

	if(file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
	{
		QString data = timestamp + title + '\n' + text + "\n\n";

		QTextStream outStream(&file);

		outStream << data;

		file.close();
	}
}
