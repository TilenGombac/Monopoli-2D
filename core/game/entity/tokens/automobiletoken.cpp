#include "automobiletoken.h"

AutomobileToken::AutomobileToken(Field *setField, QWidget *parent)
	: Token(setField, parent)
{
	tokenType = TokenType::AUTOMOBILE;

	// Default time per field for this token (milliseconds)
	timePerField = 150;

	pixmap = TokenPixmapUtils::getAutomobilePixmap();

	// Not in the Token constructor, because the pixmap is not yet
	// defined at that point
	this->initializeGeometry();
}

void AutomobileToken::move(Field *targetField)
{
	SoundManager::play(Sound::AUTOMOBILE);

	moving = true;

	QPropertyAnimation *animation = new QPropertyAnimation(this, "geometry");

	this->animationSetup(animation, targetField);

	animation->setEasingCurve(QEasingCurve::Linear);

	animation->start(QAbstractAnimation::DeleteWhenStopped);
}
