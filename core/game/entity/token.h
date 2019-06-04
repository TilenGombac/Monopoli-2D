#ifndef TOKEN_H
#define TOKEN_H

#include <QWidget>
#include <QPropertyAnimation>

#include "core/utils/tokenUtils/tokenpixmaputils.h"

#include "core/sounds/soundmanager.h"

class Field;

enum class TokenType
{
	SCOTTISH_TERRIER = 0,
	AUTOMOBILE       = 1,
	BATTLESHIP       = 2,
	HOWITZER         = 3,
	THIMBLE          = 4,
	TOP_HAT          = 5
};

class Token : public QWidget
{
	Q_OBJECT

public:
	Token(Field *setField, QWidget *parent = 0);

	// getters and setters
	TokenType getTokenType();
	QPixmap getPixmap();
	Field* getField();
	int getTimePerField();
	bool getMoving();

	// ONLY used in case of go to jail
	void setField(Field *setField);

	void setTimePerField(const int &time);

	void initializeGeometry();

	virtual void move(Field *targetField) = 0;

protected:
	// The current position of the token
	Field *field;

	// Visual representation of the token
	QPixmap pixmap;

	// Used when dynamic_cast is needed
	TokenType tokenType;

	// The amount of time the token uses to move over a field
	int timePerField;

	bool moving;


	void animationSetup(QPropertyAnimation *animation, Field *target);

	void paintEvent(QPaintEvent *event);

	void mouseReleaseEvent(QMouseEvent *event);

public slots:
	void onTargetReached();

signals:
	// Emit when the target field is reached - when animation stops
	void reachedTarget();

};

#endif // TOKEN_H
