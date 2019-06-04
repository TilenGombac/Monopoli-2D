#ifndef CARDSMANAGER_H
#define CARDSMANAGER_H

#include <QVector>

class Card;
class QWidget;

class CardsManager
{
public:
	static QVector<Card*> getDefaultCards(QWidget* parent = 0);

};

#endif // CARDSMANAGER_H
