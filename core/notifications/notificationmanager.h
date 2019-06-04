#ifndef NOTIFICATIONMANAGER_H
#define NOTIFICATIONMANAGER_H

#include "core/elements/notification.h"

class NotificationManager
{
	static QWidget* mainWindow;
	static Notification* notification;

public:
	static void setMainWindow(QWidget* setMainWindow);
	static void pushNotification(const QString &title,
									const QString &description);
	static void pushNotification(const QString &title,
									const QString &description,
										const NotificationType &type);

};

#endif // NOTIFICATIONMANAGER_H
