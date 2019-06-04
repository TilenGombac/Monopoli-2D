#ifndef FIELDCOLORDISPLAYUTILS_H
#define FIELDCOLORDISPLAYUTILS_H

class QColor;
class QPainter;

class FieldColorDisplayUtils
{
public:
	static void drawColorBox(QPainter *painter, const QColor &color,
								const bool &sidebar);

};

#endif // FIELDCOLORDISPLAYUTILS_H
