#include "playerpicker.h"

#include <QLabel>
#include <QToolButton>

#include "core/game/player/player.h"
#include "core/utils/windowUtils/windowutils.h"

PlayerPicker::PlayerPicker(const QVector<Player*> &setPlayers,
							const int &providerId, QWidget *parent)
	: QWidget(parent)
{
	this->setWindowModality(Qt::WindowModal);
	this->setWindowFlags(Qt::WindowStaysOnTopHint);
	this->setAttribute(Qt::WA_DeleteOnClose);

	this->setFixedSize(250, 60);

	this->setWindowTitle("Izbor igralca");

	QLabel *label = new QLabel("Izberi igralca", this);

	label->setAlignment(Qt::AlignCenter);
	label->setGeometry(0, 0, this->width(), this->height());
	label->setFont(QFont("Verdana", 14));
	label->show();

	players = setPlayers;

	QToolButton *button;

	Player *player;

	int id;
	int j = 0;

	for(int i = 0; i < players.size(); i++)
	{
		player = players.at(i);

		if(player->getId() != providerId)
		{
			j++;

			id = player->getId();

			button = new QToolButton(this);

			button->setGeometry(0, j * 60, this->width(), 60);
			button->setObjectName(QString::number(id));
			button->setText(player->getName());
			button->setFont(QFont("Verdana", 13));

			connect(button, SIGNAL(clicked()),
						this, SLOT(handleButtonClick()));
		}
	}

	this->setFixedSize(250, 60 * (j + 1));

	WindowUtils::centerToScreen(this);
}

void PlayerPicker::handleButtonClick()
{
	QToolButton *sender = qobject_cast<QToolButton*> (QObject::sender());

	int id = sender->objectName().toInt();

	for(Player *player : players)
	{
		if(player->getId() == id)
		{
			emit playerPicked(player);

			this->hide();

			return;
		}
	}
}
