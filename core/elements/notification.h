#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <QWidget>

class QPropertyAnimation;

enum class NotificationType
{
	ERROR,
	WARNING,
	INFORMATION
};

struct NotificationStruct
{
	QString title;
	QString description;
	NotificationType type;
};

class Notification : public QWidget
{
	Q_OBJECT

	QVector<NotificationStruct> queue;

	NotificationStruct _current;

	QPropertyAnimation* animation;

public:
	Notification(QWidget* parent);
	void add(const QString &title, const QString &description,
				const NotificationType &type = NotificationType::ERROR);

private slots:
	void showNext();

protected:
	void paintEvent(QPaintEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);

};

#endif // NOTIFICATION_H
