#include "notificationmanager.h"

void NotificationManager::setMainWindow(QWidget *setMainWindow)
{
	mainWindow = setMainWindow;

	notification = new Notification(setMainWindow);
}

void NotificationManager::pushNotification(const QString &title,
											const QString &description)
{

	// Check if the NotificationManager has been initialized
	if(notification != nullptr)
	{
		notification->add(title, description);
	}
}

void NotificationManager::pushNotification(const QString &title,
											const QString &description,
											  const NotificationType &type)
{
	// Check if the NotificationManager has been initialized
	if(notification != nullptr)
	{
		notification->add(title, description, type);
	}
}

QWidget* NotificationManager::mainWindow = nullptr;

Notification* NotificationManager::notification = nullptr;
