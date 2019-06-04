#include "playerslist.h"

#include <QPainter>

#include "core/game/board/board.h"
#include "core/game/player/player.h"
#include "core/game/entity/token.h"
#include "core/game/managers/boardmanager.h"

PlayersList::PlayersList(BoardManager *setBoardManager, QWidget *parent)
	: QWidget(parent)
{
	boardManager = setBoardManager;

	this->setGeometry(400, 130, 400, 400);

	this->show();
}

QRect PlayersList::playerRect(Player *player)
{
	int w = 335;
	int h = 65;

	int x = 0;
	int y = player->getId() * h;

	return QRect(x, y, w, h);
}

void PlayersList::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);

	QPainter painter(this);

	painter.setFont(QFont("Verdana", 24));

	int i = 0;

	for(Player *player : boardManager->getBoard()->getPlayers())
	{
		painter.drawText(playerRect(player), Qt::AlignTop,
							player->getName());

		QPixmap pixmap = player->getToken()->getPixmap();

		pixmap = pixmap.scaled(55, 55, Qt::KeepAspectRatio,
								Qt::SmoothTransformation);

		painter.drawPixmap(345, 65 * i, pixmap.width(), pixmap.height(),
							pixmap);

		i++;
	}
}
