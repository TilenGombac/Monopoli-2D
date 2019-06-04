#include "boarddisplay.h"

#include <QPainter>

#include "core/game/entity/fields/possessablefield.h"
#include "core/utils/boardUtils/boarddisplayutils.h"
#include "core/game/managers/boardmanager.h"
#include "core/game/board/board.h"
#include "core/game/player/player.h"
#include "core/game/entity/token.h"
#include "core/game/entity/field.h"
#include "core/game/entity/card.h"

BoardDisplay::BoardDisplay(BoardManager *manager, QWidget *parent)
	: QWidget(parent)
{	
	boardManager = manager;

	this->setGeometry(0, 0, parent->width(), parent->height());
}

BoardManager* BoardDisplay::getBoardManager()
{
	return boardManager;
}

void BoardDisplay::showBoard()
{
	this->show();

	for(Field *field : boardManager->getBoard()->getFields())
	{
		field->show();
	}

	for(Player *player : boardManager->getBoard()->getPlayers())
	{
		player->getToken()->show();
	}
}

void BoardDisplay::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);

	QPainter painter(this);

	for(Field *field : boardManager->getBoard()->getFields())
	{
		if(field->getPossessable())
		{
			PossessableField *tmp = dynamic_cast<PossessableField*> (field);

			int playerId = tmp->getOwnerId();

			if(playerId != -1)
			{
				Player *player = boardManager->getPlayer(playerId);

				if(player != nullptr)
				{
					int fieldId = tmp->getId();

					QString initial = player->getName().at(0);

					BoardDisplayUtils::drawOwnerBox(&painter, playerId,
													fieldId, initial,
													tmp->getMortgaged());
				}
			}
		}
	}

	BoardDisplayUtils::drawBorders(&painter);
	BoardDisplayUtils::drawText(&painter, "Monopoli 2D");
}
