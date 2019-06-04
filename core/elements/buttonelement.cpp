#include "buttonelement.h"

#include <QMouseEvent>
#include <QEvent>

#include "core/utils/resourceutils.h"

ButtonElement::ButtonElement(QWidget *parent)
	: QPushButton(parent)
{
	QString defaultStylePath = ":/css/css/buttons/buttonElement.css";

	this->setStyleSheet(ResourceUtils::getStyleSheet(defaultStylePath));
	this->setCursor(Qt::PointingHandCursor);
}

void ButtonElement::setTextColor(const QColor &color)
{
	textColor = color;

	this->changeTextColor(textColor);
}

void ButtonElement::setHoverTextColor(const QColor &color)
{
	hoverTextColor = color;
}

void ButtonElement::setPressedTextColor(const QColor &color)
{
	pressedTextColor = color;
}

void ButtonElement::changeTextColor(const QColor &color)
{
	QPalette palette;

	palette.setColor(QPalette::ButtonText, color);

	this->setPalette(palette);
}

void ButtonElement::enterEvent(QEvent *event)
{
	this->changeTextColor(hoverTextColor);

	Q_UNUSED(event);
}

void ButtonElement::leaveEvent(QEvent *event)
{
	this->changeTextColor(textColor);

	Q_UNUSED(event);
}

void ButtonElement::mousePressEvent(QMouseEvent *event)
{
	this->changeTextColor(pressedTextColor);

	if(event->button() == Qt::LeftButton)
	{
		emit pressed();
	}
}

void ButtonElement::mouseReleaseEvent(QMouseEvent *event)
{
	this->changeTextColor(hoverTextColor);

	bool mouseInsideBtnArea = this->rect().contains(event->pos());

	if(event->button() == Qt::LeftButton && mouseInsideBtnArea)
	{
		emit clicked(true);
	}
}
