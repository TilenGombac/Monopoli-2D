#include "card.h"

#include <QTimer>

#include "core/utils/constantutils.h"
#include "core/elements/buttonelement.h"
#include "core/utils/windowUtils/windowutils.h"
#include "core/utils/errorUtils/errorhandler.h"

#include "core/sounds/soundmanager.h"

Card::Card(const int &setId, QWidget* parent)
	: QWidget(parent)
{
	id = setId;

	invisible = false;

	hideTimer = new QTimer(this);
	hideTimer->setInterval(5000);
	hideTimer->stop();

	connect(hideTimer, SIGNAL(timeout()),
				this, SLOT(hide()));
	connect(hideTimer, SIGNAL(timeout()),
				hideTimer, SLOT(stop()));

	Action tmpAction;
	tmpAction.setNoAction();

	action        = tmpAction;
	specialAction = SpecialAction::NONE;

	this->setFixedSize(350, 200);

	WindowUtils::centerToScreen(this);

	exitButton = new ButtonElement(this);
	exitButton->setGeometry(this->width() - 30, 0, 30, 30);
	exitButton->setFont(QFont("Verdana", 16));
	exitButton->setText("X");
	exitButton->setTextColor(Qt::black);
	exitButton->setHoverTextColor(QColor(50, 50, 50));
	exitButton->setPressedTextColor(QColor(100, 100, 100));
	exitButton->show();

	connect(exitButton, SIGNAL(clicked()), this, SLOT(hide()));

	this->hide();
	this->setWindowModality(Qt::WindowModal);
	this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
}

CardType Card::getCardType()
{
	return cardType;
}


// getters and setters
int Card::getId()
{
	return id;
}

QString Card::getText()
{
	return text;
}

QString Card::getName()
{
	return name;
}

QImage Card::getImage()
{
	return image;
}

Action Card::getAction()
{
	return action;
}

SpecialAction Card::getSpecialAction()
{
	return specialAction;
}


void Card::setId(const int &setId)
{
	id = setId;
}

void Card::setText(const QString &setText)
{
	text = setText;
}

void Card::setName(const QString &setName)
{
	name = setName;
}

void Card::setImage(const QImage &setImage)
{
	image = setImage;
}

void Card::setAction(const Action &setAction)
{
	action = setAction;
}

void Card::setInvisible(const bool &setInvisible)
{
	invisible = setInvisible;
}

void Card::setSpecialAction(const SpecialAction &setSpecialAction)
{
	specialAction = setSpecialAction;
}

void Card::showEvent(QShowEvent *event)
{
	QWidget::showEvent(event);

	if(invisible)
	{
		this->hide();
	}
	else
	{
		SoundManager::play(Sound::CARD);

		hideTimer->start();

		QString message;
		if(this->getCardType() == CardType::CHANCE)
		{
			message = "Type: CHANCE, ";
		}
		else
		{
			message = "Type: COMMUNITY_CHEST, ";
		}

		message += "text: " + this->text;

		ErrorHandler::log("Card shown", message);
	}
}
