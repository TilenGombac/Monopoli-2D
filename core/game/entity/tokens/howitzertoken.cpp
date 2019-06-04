#include "howitzertoken.h"

#include <QPropertyAnimation>

HowitzerToken::HowitzerToken(Field *setField, QWidget *parent)
	: Token(setField, parent)
{
	tokenType = TokenType::HOWITZER;

	// Default time per field for this token (milliseconds)
	timePerField = 300;

	pixmap = TokenPixmapUtils::getHowitzerPixmap();

	// Not in the Token constructor, because the pixmap is not yet
	// defined at that point
	this->initializeGeometry();
}

void HowitzerToken::move(Field *targetField)
{
	SoundManager::play(Sound::HOWITZER);

	moving = true;

	QPropertyAnimation *animation = new QPropertyAnimation(this, "geometry");

	this->animationSetup(animation, targetField);

	animation->setEasingCurve(QEasingCurve::Linear);

	animation->start(QAbstractAnimation::DeleteWhenStopped);
}
