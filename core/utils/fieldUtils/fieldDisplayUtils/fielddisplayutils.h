#ifndef FIELDDISPLAYUTILS_H
#define FIELDDISPLAYUTILS_H

#include <QSize>
#include <QFont>

class FieldDisplayUtils
{
public:
	static void drawField(QPainter *painter, const int &fieldId,
							const bool &sidebar);

	static QFont getFont(const bool &sidebar);
	static QSize getSize(const int &fieldId, const bool &sidebar);

	static int getMultiplier(const bool &sidebar);

	static void transform(QPainter *painter, const int &fieldId);
	static void drawFieldRect(QPainter *painter, const int &fieldId,
								const bool &sidebar);

};

#endif // FIELDDISPLAYUTILS_H
