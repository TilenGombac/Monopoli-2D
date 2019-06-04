#ifndef CARD_H
#define CARD_H

#include <QWidget>

#include "core/game/action.h"

class QTimer;
class ButtonElement;

enum class CardType
{
	COMMUNITY_CHEST = 0,
	CHANCE          = 1
};

enum class SpecialAction
{
	NONE     = 0,
	REPAIRS  = 1, // Payment for each house / hotel
	CHAIRMAN = 2, // Payment to each player
	BIRTHDAY = 3  // Payment from each player
};

class Card : public QWidget
{
	Q_OBJECT

	QTimer *hideTimer;
	ButtonElement *exitButton;

public:
	Card(const int &setId, QWidget* parent = 0);
	CardType getCardType();

	// getters and setters
	int getId();
	QString getText();
	QString getName();
	QImage getImage();
	Action getAction();
	SpecialAction getSpecialAction();

	void setId(const int &setId);
	void setText(const QString &setText);
	void setName(const QString &setName);
	void setImage(const QImage &setImage);
	void setAction(const Action &setAction);
	void setInvisible(const bool &setInvisible);
	void setSpecialAction(const SpecialAction &setSpecialAction);

protected:
	void showEvent(QShowEvent *event);

protected:
	int id;
	bool invisible;
	CardType cardType;
	QString text;
	QString name;
	QImage image;
	Action action;
	SpecialAction specialAction;

};

#endif // CARD_H
