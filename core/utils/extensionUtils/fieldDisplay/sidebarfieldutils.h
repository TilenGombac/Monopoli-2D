#ifndef SIDEBARFIELDUTILS_H
#define SIDEBARFIELDUTILS_H

#include <QRect>

class SidebarFieldUtils
{
public:
	static QRect getGeometry();
	static QRect getFieldGeometry(const int &fieldId);
	static QRect getRentGeometry();

};

#endif // SIDEBARFIELDUTILS_H
