#include "participantsview.h"

#include <QTimer>
#include <QPainter>

#include "core/game/entity/token.h"
#include "core/game/player/player.h"
#include "core/utils/constantutils.h"
#include "core/utils/playerUtils/playerutils.h"

ParticipantsView::ParticipantsView(const bool &setMultiplayer,
									QWidget *parent)
	: QWidget(parent)
{
	multiplayer = setMultiplayer;

	playerTurn = -1;

	this->setGeometry(0, 60, ConstantUtils::SIDEBAR_WIDTH, 281);

	this->show();
}

ParticipantsView::ParticipantsView(const QVector<Player*> &players,
									QWidget *parent)
	: QWidget(parent)
{
	multiplayer = false;

	playerTurn = -1;

	this->setGeometry(0, 60, ConstantUtils::SIDEBAR_WIDTH, 281);

	this->show();

	for(Player *player : players)
	{
		participants.append(QPair<Player*, int> (player, 0));

		connect(player, SIGNAL(balanceChanged()),
					this, SLOT(refresh()));

		connect(player->getToken(), SIGNAL(reachedTarget()),
					this, SLOT(refresh()));
	}
}

QVector<Player*> ParticipantsView::getParticipants()
{
	QVector<Player*> players;

	for(QPair<Player*, int> participantPair : participants)
	{
		players.append(participantPair.first);
	}

	return players;
}

void ParticipantsView::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);

	QPainter painter(this);

	QColor red(255, 80, 80);
	QColor orange(255, 160, 0);
	QColor green(0, 200, 60);
	QColor lightGray(240, 240, 240);

	painter.setFont(QFont("Verdana", 13));

	int w = this->width() - 2;
	int x = 1;

	// Column height
	int h = 36;

	int padding = 6;

	int nameWidth      = 5 * h;
	int pingWidth      = h;
	int balanceWidth   = 4 * h;

	if(multiplayer)
	{
		nameWidth      = 4 * h;
		pingWidth      = 3 * h;
		balanceWidth   = 3 * h;
	}

	int indicatorWidth = 4;

	int indicatorX = padding + x + 2;

	int nameX = indicatorX + indicatorWidth + padding;
	int balanceX = nameX + nameWidth + padding;
	int pingX = balanceX + balanceWidth + padding;

	int y;
	int i = 0;

	QString name;
	QString ping;
	QString balance;

	Player* player;

	for(QPair<Player*, int> participantPair : participants)
	{
		player = participantPair.first;

		y = i * h;

		// No border drawing
		painter.setPen(Qt::NoPen);

		painter.setBrush(PlayerUtils::getPlayerColor(player->getId()));

		// Draw player color rect
		painter.drawRect(x, y, w, h);

		if(player->getId() == playerTurn)
		{
			// Draw player turn indicator
			int indicatorY = y + (h - indicatorWidth) / 2;

			painter.setBrush(Qt::white);
			painter.setPen(Qt::white);

			painter.drawEllipse(indicatorX, indicatorY, indicatorWidth,
									indicatorWidth);
		}

		if(player->getJailed())
		{
			painter.setPen(red);
		}
		else
		{
			painter.setPen(lightGray);
		}

		name    = player->getName();
		balance = QString::number(player->getAccountBalance()) + " €";

		painter.drawText(nameX, y, nameWidth, h, Qt::AlignVCenter, name);

		painter.drawText(balanceX, y, balanceWidth, h, Qt::AlignVCenter,
							balance);

		if(multiplayer)
		{
			int pingNum = participantPair.second;

			ping = QString::number(pingNum) + "ms";

			if(pingNum <= 100)
			{
				painter.setPen(green);
			}
			else if((pingNum > 100) && (pingNum < 250))
			{
				painter.setPen(orange);
			}
			else
			{
				painter.setPen(red);
			}

			painter.drawText(pingX, y, pingWidth, h, Qt::AlignVCenter,
								ping);
		}

		i++;
	}
}

void ParticipantsView::addParticipant(Player *player)
{
	int count = 0;
	for(QPair<Player*, int> participantPair : participants)
	{
		if(participantPair.first->getId() == player->getId())
		{
			count++;
		}
	}

        // Add the player if hasn't been added yet
	if(count == 0)
	{
		participants.append(QPair<Player*, int> (player, -1));

		connect(player, SIGNAL(balanceChanged()),
				this, SLOT(refresh()));

		connect(player->getToken(), SIGNAL(reachedTarget()),
						this, SLOT(refresh()));

		this->refresh();
	}
}

void ParticipantsView::removeParticipant(Player *player)
{
	disconnect(player, SIGNAL(balanceChanged()),
				this, SLOT(refresh()));

	disconnect(player->getToken(), SIGNAL(reachedTarget()),
				this, SLOT(refresh()));

	for(QPair<Player*, int> participantPair : participants)
	{
		if(participantPair.first->getId() == player->getId())
		{
			participants.removeAll(participantPair);
		}
	}

	this->refresh();
}

void ParticipantsView::setPlayerTurn(Player *player)
{
	this->playerTurn = player->getId();

	this->repaint();
}

void ParticipantsView::refreshPing(int playerId, int ping)
{
	for(int i = 0; i < participants.size(); i++)
	{
		if(participants[i].first->getId() == playerId)
		{
			participants[i].second = ping;

			this->repaint();

			return;
		}
	}
}

void ParticipantsView::refresh()
{
	QTimer *timer = new QTimer(this);

	connect(timer, SIGNAL(timeout()), this, SLOT(repaint()));
	connect(timer, SIGNAL(timeout()), timer, SLOT(stop()));
	connect(timer, SIGNAL(timeout()), timer, SLOT(deleteLater()));

	/*
	 * This timer allows the application to process the purchase / mortgage
	 * and therefore prevent getNetWorth() to return a value before
	 * a purchase / mortgage has gone through
	 */
	timer->start(10);
}
