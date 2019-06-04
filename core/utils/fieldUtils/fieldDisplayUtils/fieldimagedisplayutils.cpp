#include "fieldimagedisplayutils.h"

#include <QPainter>

#include "core/utils/fieldUtils/fieldDisplayUtils/fielddisplayutils.h"

void FieldImageDisplayUtils::drawImage(QPainter *painter, const int &fieldId,
										QImage image, const bool &sidebar)
{
	QSize size = FieldDisplayUtils::getSize(fieldId, sidebar);

	int width  = size.width();
	int height = size.height();

	// -1 prevents overlapping the borders
	int displayLength = width - 1;

	int x = 1;
	int y = 1;

	// Regular field
	if(fieldId % 10 != 0)
	{
		int multiplier = FieldDisplayUtils::getMultiplier(sidebar);

		y += 10 * multiplier;

		displayLength -= 15 + 10 * multiplier;
	}

	image = image.scaled(displayLength, displayLength - 1);

	x += ((width - displayLength) / 2);
	y += ((height - displayLength) / 2);

	painter->drawImage(x, y, image);
}
