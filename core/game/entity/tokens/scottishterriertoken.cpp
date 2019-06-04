#include "scottishterriertoken.h"

#include <QPropertyAnimation>

ScottishTerrierToken::ScottishTerrierToken(Field *setField, QWidget *parent)
	: Token(setField, parent)
{
	tokenType = TokenType::SCOTTISH_TERRIER;

	// Default time per field for this token (milliseconds)
	timePerField = 200;

	pixmap = TokenPixmapUtils::getScottishTerrierPixmap();

	// Not in the Token constructor, because the pixmap is not yet
	// defined at that point
	this->initializeGeometry();
}

void ScottishTerrierToken::move(Field *targetField)
{
	SoundManager::play(Sound::SCOTTISH_TERRIER);

	moving = true;

	QPropertyAnimation *animation = new QPropertyAnimation(this, "geometry");

	this->animationSetup(animation, targetField);

	animation->setEasingCurve(QEasingCurve::Linear);

	animation->start(QAbstractAnimation::DeleteWhenStopped);
}
