#include "buildingdisplayutils.h"

#include <QPainter>

#include "core/utils/constantutils.h"

void BuildingDisplayUtils::drawBuildings(QPainter *painter,
											const int &number,
												const bool &sidebar)
{
	if(number == 0)
		return;

	// Hotel
	if(number == 5)
		drawHotel(painter, sidebar);

	// House(s)
	else
		drawHouses(painter, number, sidebar);
}

void BuildingDisplayUtils::drawHotel(QPainter *painter, const bool &sidebar)
{
	QString path;

	if(sidebar)
		path = ":/img/img/buildings/hotel_large.png";
	else
		path = ":/img/img/buildings/hotel_small.png";

	QPixmap hotelPixmap(path);

	int w = hotelPixmap.width();
	int h = hotelPixmap.height();

	int x = (getFieldWidth(sidebar) - w) / 2;
	int y = (getColorBoxHeight(sidebar) - h) / 2;

	painter->drawPixmap(x, y, w, h, hotelPixmap);
}

void BuildingDisplayUtils::drawHouses(QPainter *painter, const int &number,
										const bool &sidebar)
{
	QString path;

	if(sidebar)
		path = ":/img/img/buildings/house_large.png";
	else
		path = ":/img/img/buildings/house_small.png";

	QPixmap housePixmap(path);

	int spacing = (4 - number) + (9 / number);

	int w = housePixmap.width();
	int h = housePixmap.height();

	int x = (getFieldWidth(sidebar) - w - (w + spacing) * (number - 1)) / 2;
	int y = (getColorBoxHeight(sidebar) - h) / 2;

	for(int i = 0; i < number; i++)
	{
		painter->drawPixmap(x, y, w, h, housePixmap);

		x += spacing + w;
	}
}

int BuildingDisplayUtils::getFieldWidth(const bool &sidebar)
{
	int fieldWidth  = ConstantUtils::FIELD_WIDTH;

	if(sidebar)
		fieldWidth *= 2;

	return fieldWidth;
}

int BuildingDisplayUtils::getColorBoxHeight(const bool &sidebar)
{
	int fieldHeight = ConstantUtils::FIELD_HEIGHT;

	if(sidebar)
		fieldHeight *= 2;

	int colorBoxHeight = fieldHeight / 4;

	return colorBoxHeight;
}
