#include "communitychestcard.h"

#include <QPainter>

CommunityChestCard::CommunityChestCard(const int &id, QWidget* parent)
	: Card(id, parent)
{
	cardType = CardType::COMMUNITY_CHEST;
}

void CommunityChestCard::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);

	QString name = "Državna blagajna";

	QPainter painter(this);

	// Background
	painter.setBrush(QColor(242, 236, 200));
	painter.drawRect(0, 0, this->width() - 1, this->height() - 1);

	painter.setFont(QFont("Georgia", 18, QFont::Bold));

	painter.drawText(0, 0, this->width(), 60,
						Qt::AlignHCenter | Qt::AlignBottom,
							name.toUpper());

	painter.setFont(QFont("Arial", 12));

	painter.drawText(0, 60, this->width(), this->height() - 60,
						Qt::AlignCenter, text.toUpper());
}
