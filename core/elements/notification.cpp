#include "notification.h"

#include <QPainter>
#include <QMouseEvent>
#include <QPropertyAnimation>

#include "core/sounds/soundmanager.h"

Notification::Notification(QWidget *parent)
	: QWidget(parent)
{
	this->show();

	int x = 1;
	int h = 60;

	this->setGeometry(x, -h, parent->width() - 2, h);

	animation = new QPropertyAnimation(this, "geometry");

	animation->setKeyValueAt(0, QRect(x, -h, this->width(), h));
	animation->setKeyValueAt(0.15, QRect(x, 0, this->width(), h));
	animation->setKeyValueAt(0.85, QRect(x, 0, this->width(), h));
	animation->setKeyValueAt(1, QRect(x, -h, this->width(), h));

	animation->setDuration(5000);

	connect(animation, SIGNAL(finished()), this, SLOT(showNext()));
}

void Notification::add(const QString &title, const QString &description,
						const NotificationType &type)
{
	NotificationStruct tmp;
	tmp.title       = title;
	tmp.description = description;
	tmp.type        = type;

	queue.append(tmp);

	this->showNext();
}

void Notification::showNext()
{
	if(animation->state() == QAbstractAnimation::Running)
	{
		return;
	}

	if(queue.size() >= 1)
	{
		_current = queue[0];

		queue.remove(0);

		this->raise();

		if(_current.type == NotificationType::ERROR)
		{
			SoundManager::play(Sound::ERROR);
		}
		else if(_current.type == NotificationType::WARNING)
		{
			SoundManager::play(Sound::WARNING);
		}
		else
		{
			SoundManager::play(Sound::INFORMATION);
		}

		animation->start();
	}
}

void Notification::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event)

	QPainter painter(this);

	NotificationType type = _current.type;

	if(type == NotificationType::ERROR)
	{
		painter.setBrush(QBrush(QColor(255, 80, 80)));
	}
	else if(type == NotificationType::WARNING)
	{
		painter.setBrush(QBrush(QColor(255, 230, 85)));
	}
	else
	{
		painter.setBrush(QBrush(QColor(0, 200, 60)));
	}

	painter.setPen(Qt::NoPen);

	QFont font = painter.font();

	font.setPointSize(13);

	painter.setFont(font);


	painter.drawRect(0, 0, this->width(), this->height());

	painter.setPen(Qt::white);

	painter.drawText(0, 30, this->width(), 30, Qt::AlignHCenter,
						_current.description);

	font.setBold(true);

	painter.setFont(font);

	painter.drawText(0, 5, this->width(), 25, Qt::AlignHCenter,
						_current.title);
}

void Notification::mouseReleaseEvent(QMouseEvent *event)
{
	if(event->button() == Qt::LeftButton)
	{
		animation->stop();

		this->setGeometry(0, -60, this->width(), 60);

		this->showNext();
	}
}
