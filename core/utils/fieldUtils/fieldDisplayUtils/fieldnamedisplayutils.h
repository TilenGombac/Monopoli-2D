#ifndef FIELDNAMEDISPLAYUTILS_H
#define FIELDNAMEDISPLAYUTILS_H

class QString;
class QPainter;

class FieldNameDisplayUtils
{
	static int NAME_OFFSET;

public:
	static void drawName(QPainter *painter, const QString &name,
							const bool &sidebar, const bool &offset = false);

};

#endif // FIELDNAMEDISPLAYUTILS_H
