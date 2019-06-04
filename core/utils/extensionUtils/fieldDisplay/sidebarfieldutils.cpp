#include "sidebarfieldutils.h"

#include "core/utils/constantutils.h"
#include "core/utils/extensionUtils/sidebar/sidebarutils.h"

QRect SidebarFieldUtils::getGeometry()
{
	// + 1 due to borders
	int len = ConstantUtils::FIELD_HEIGHT * 2 + 1;

	int sidebar_W  = ConstantUtils::SIDEBAR_WIDTH;
	int sidebar_H = SidebarUtils::getGeometry().height();

	return QRect(0, (sidebar_H - len), sidebar_W, len);
}

QRect SidebarFieldUtils::getFieldGeometry(const int &fieldId)
{
	// + 1 due to borders
	int len = ConstantUtils::FIELD_HEIGHT * 2 + 1;

	int w = ConstantUtils::FIELD_WIDTH * 2;
	int h = ConstantUtils::FIELD_HEIGHT * 2;

	if(fieldId % 10 == 0)
	{
		w = ConstantUtils::FIELD_HEIGHT * 2;
	}

	int x = ConstantUtils::SIDEBAR_WIDTH - len;
	x += h - w;

	return QRect(x, 0, w, h);
}

QRect SidebarFieldUtils::getRentGeometry()
{
	int sidebarW = ConstantUtils::SIDEBAR_WIDTH;

	int w = sidebarW - ConstantUtils::FIELD_WIDTH * 2;
	int h = ConstantUtils::FIELD_HEIGHT * 2;
	int x = 8;
	int y = 0;

	return QRect(x, y, w, h);
}
