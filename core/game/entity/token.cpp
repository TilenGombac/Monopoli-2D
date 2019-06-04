#include "token.h"

#include <QPainter>
#include <QMouseEvent>
#include <QPropertyAnimation>

#include "core/utils/tokenUtils/tokengeometryutils.h"
#include "core/utils/tokenUtils/tokenanimationutils.h"
#include "core/game/entity/fields/cardfield.h"
#include "core/game/entity/fields/companyfield.h"
#include "core/game/entity/fields/cornerfield.h"
#include "core/game/entity/fields/propertyfield.h"
#include "core/game/entity/fields/taxfield.h"

#include "core/utils/errorUtils/errorhandler.h"

Token::Token(Field *setField, QWidget *parent)
	: QWidget(parent)
{
	field = setField;

	moving = false;
}


// Getters and setters
TokenType Token::getTokenType()
{
	return tokenType;
}

QPixmap Token::getPixmap()
{
	return pixmap;
}

Field* Token::getField()
{
	return field;
}

int Token::getTimePerField()
{
	return timePerField;
}

bool Token::getMoving()
{
	return moving;
}

void Token::setField(Field *setField)
{
	field = setField;

	this->initializeGeometry();

	ErrorHandler::log("Token setField", QString::number(field->getId()));

	emit reachedTarget();
}

void Token::setTimePerField(const int &time)
{
	timePerField = time;
}


void Token::initializeGeometry()
{
	QRect rect = TokenGeometryUtils::getRect(field->getId(), pixmap);

	this->setGeometry(rect);
}


void Token::animationSetup(QPropertyAnimation *animation, Field *target)
{
	ErrorHandler::log("Token animationSetup", "TargetID: "
						+ QString::number(target->getId()));

	int currentId = field->getId();
	int targetId  = target->getId();

	int duration = TokenAnimationUtils::getDuration(currentId, targetId,
													  timePerField);
	animation->setDuration(duration);

	QVariantAnimation::KeyValues keyValues =
		TokenAnimationUtils::generateKeyValues(targetId, currentId, pixmap);

	animation->setKeyValues(keyValues);

	connect(animation, SIGNAL(finished()), this, SLOT(onTargetReached()));
	connect(animation, SIGNAL(finished()), target, SLOT(sendClicked()));
	connect(animation, SIGNAL(finished()), animation, SLOT(deleteLater()));

	field = target;
}

void Token::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);

	QPainter painter(this);

	painter.drawPixmap(pixmap.rect(), pixmap);
}

void Token::mouseReleaseEvent(QMouseEvent *event)
{
	if(event->button() == Qt::LeftButton)
	{
		field->sendClicked();
	}
}

void Token::onTargetReached()
{
	moving = false;

	field->sendClicked();

	emit reachedTarget();
}
