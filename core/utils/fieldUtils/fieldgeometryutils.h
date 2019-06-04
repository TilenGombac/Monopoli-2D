#ifndef FIELDGEOMETRYUTILS_H
#define FIELDGEOMETRYUTILS_H

#include <QRect>

class FieldGeometryUtils
{
public:
	static QRect getRect(const int &fieldId);
	static QRect getRotatedRect(const int &fieldId);
	static QSize getSize(const int &fieldId);
	static QSize getRotatedSize(const int &fieldId);
	static QPoint getPosition(const int &fieldId);

private:
	static int getPositionX(const int &fieldId);
	static int getPositionY(const int &fieldId);
};

#endif // FIELDGEOMETRYUTILS_H
