#ifndef BOARDDISPLAYUTILS_H
#define BOARDDISPLAYUTILS_H

class QPainter;
class QString;

class BoardDisplayUtils
{
public:
	static void drawText(QPainter *painter, const QString &text);
	static void drawBorders(QPainter *painter);
	static void drawOwnerBox(QPainter *painter, const int &ownerId,
								const int &fieldId, const QString &initial,
									const bool &mortgaged);

};

#endif // BOARDDISPLAYUTILS_H
