#ifndef BUTTONELEMENT_H
#define BUTTONELEMENT_H

#include <QPushButton>

class ButtonElement : public QPushButton
{
	QColor textColor;
	QColor hoverTextColor;
	QColor pressedTextColor;

public:
	ButtonElement(QWidget *parent = 0);
	void setTextColor(const QColor &color);
	void setHoverTextColor(const QColor &color);
	void setPressedTextColor(const QColor &color);

private:
	void changeTextColor(const QColor &color);

protected:
	virtual void enterEvent(QEvent *event);
	virtual void leaveEvent(QEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);

};

#endif // BUTTONELEMENT_H
