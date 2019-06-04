#ifndef TOKENGEOMETRYUTILS_H
#define TOKENGEOMETRYUTILS_H

#include <QPixmap>

class TokenGeometryUtils
{
public:
	static QRect getRect(const int &fieldId, const QPixmap &pixmap);
	static QSize getSize(const QPixmap &pixmap);
	static QPoint getPosition(const int fieldId, const QPixmap &pixmap);

private:
	static int getPositionX(const int fieldId, const QPixmap &pixmap);
	static int getPositionY(const int fieldId, const QPixmap &pixmap);

};

#endif // TOKENGEOMETRYUTILS_H
