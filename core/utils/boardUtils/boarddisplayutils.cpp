#include "boarddisplayutils.h"

#include <QPainter>

#include "core/game/entity/field.h"
#include "core/utils/constantutils.h"
#include "core/utils/boardUtils/boardutils.h"
#include "core/utils/playerUtils/playerutils.h"
#include "core/utils/fieldUtils/fieldgeometryutils.h"
#include "core/utils/fieldUtils/fieldDisplayUtils/fielddisplayutils.h"

void BoardDisplayUtils::drawText(QPainter *painter, const QString &text)
{
	QFont font;
	font.setPointSize(48);

	QRect boardRect = BoardUtils::getBoardRect();

	// to keep center after rotation
	int dx = boardRect.width() / 2;
	int dy = -199; // works...

	painter->translate(dx, dy);
	painter->rotate(45);

	painter->setFont(font);
	painter->drawText(boardRect, Qt::AlignCenter, text);
}

void BoardDisplayUtils::drawBorders(QPainter *painter)
{
	QRect boardRect = BoardUtils::getBoardRect();

	int board_W = boardRect.width();
	int board_H = boardRect.height();

	int field_H = ConstantUtils::FIELD_HEIGHT;

	// Left row's right border
	painter->drawLine(field_H, field_H, field_H, (board_H - field_H));

	// Top row's bottom border
	painter->drawLine(field_H, field_H, (board_H - field_H), field_H);

	// Right row's right border
	//painter->drawLine((board_W - 1), 0, (board_W - 1), board_H);

	// Bottom row's bottom border
	painter->drawLine(0, (board_H - 1), board_W, (board_H - 1));
}

void BoardDisplayUtils::drawOwnerBox(QPainter *painter, const int &ownerId,
										const int &fieldId,
											const QString &initial,
												const bool &mortgaged)
{
	painter->setBrush(PlayerUtils::getPlayerColor(ownerId));

	if(mortgaged)
	{
		painter->setBrush(Qt::white);
	}

	int fieldWidth  = ConstantUtils::FIELD_WIDTH;
	int fieldHeight = ConstantUtils::FIELD_HEIGHT;

	int boardLength = BoardUtils::getBoardSize().width();

	int a = 24;

	int x = 0;
	int y = 0;

	int offset = (fieldWidth - a) / 2;

	if((fieldId > 0) && (fieldId < 10))
	{
		x = boardLength;

		x -= (fieldHeight + offset + a + fieldWidth * (fieldId - 1));

		y = boardLength - fieldHeight - a;
	}

	else if((fieldId > 10) && (fieldId < 20))
	{
		int id = fieldId % 10;

		x = fieldHeight;

		y = boardLength;

		y -= (fieldHeight + offset + a + fieldWidth * (id - 1));
	}

	else if((fieldId > 20) && (fieldId < 30))
	{
		int id = fieldId % 10;

		x = fieldHeight + offset + fieldWidth * (id - 1);

		y = fieldHeight;
	}

	else if((fieldId > 30) && (fieldId < 40))
	{
		int id = fieldId % 10;

		x = boardLength - fieldHeight - a;

		y = fieldHeight + offset + fieldWidth * (id - 1);
	}

	painter->drawRect(x, y, a, a);

	painter->setPen(Qt::white);
	painter->drawText(x, y, a, a, Qt::AlignCenter, initial);
	painter->setPen(Qt::black);
}
