#include "battleshiptoken.h"

#include <QPropertyAnimation>

BattleshipToken::BattleshipToken(Field *setField, QWidget *parent)
	: Token(setField, parent)
{
	tokenType = TokenType::BATTLESHIP;

	// Default time per field for this token (milliseconds)
	timePerField = 250;

	pixmap = TokenPixmapUtils::getBattleshipPixmap();

	// Not in the Token constructor, because the pixmap is not yet
	// defined at that point
	this->initializeGeometry();
}

void BattleshipToken::move(Field *targetField)
{
	SoundManager::play(Sound::BATTLESHIP);

	moving = true;

	QPropertyAnimation *animation = new QPropertyAnimation(this, "geometry");

	this->animationSetup(animation, targetField);

	animation->setEasingCurve(QEasingCurve::Linear);

	animation->start(QAbstractAnimation::DeleteWhenStopped);
}
