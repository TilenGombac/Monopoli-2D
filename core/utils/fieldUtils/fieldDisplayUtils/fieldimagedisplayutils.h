#ifndef FIELDIMAGEDISPLAYUTILS_H
#define FIELDIMAGEDISPLAYUTILS_H

class QImage;
class QPainter;

class FieldImageDisplayUtils
{
public:
	static void drawImage(QPainter *painter, const int &fieldId,
							QImage image, const bool &sidebar);

};

#endif // FIELDIMAGEDISPLAYUTILS_H
