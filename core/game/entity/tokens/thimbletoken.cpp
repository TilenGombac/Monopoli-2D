#include "thimbletoken.h"

#include <QPropertyAnimation>

ThimbleToken::ThimbleToken(Field *setField, QWidget *parent)
	: Token(setField, parent)
{
	tokenType = TokenType::THIMBLE;

	// Default time per field for this token (milliseconds)
	timePerField = 350;

	pixmap = TokenPixmapUtils::getThimblePixmap();

	// Not in the Token constructor, because the pixmap is not yet
	// defined at that point
	this->initializeGeometry();
}

void ThimbleToken::move(Field *targetField)
{
	moving = true;

	QPropertyAnimation *animation = new QPropertyAnimation(this, "geometry");

	this->animationSetup(animation, targetField);

	animation->setEasingCurve(QEasingCurve::Linear);

	animation->start(QAbstractAnimation::DeleteWhenStopped);
}
