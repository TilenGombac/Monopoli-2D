#ifndef BUILDINGDISPLAYUTILS_H
#define BUILDINGDISPLAYUTILS_H

class QPainter;

class BuildingDisplayUtils
{
public:
	static void drawBuildings(QPainter *painter, const int &number,
								const bool &sidebar);

private:
	static void drawHotel(QPainter *painter, const bool &sidebar);
	static void drawHouses(QPainter *painter, const int &number,
							const bool &sidebar);

	static int getFieldWidth(const bool &sidebar);
	static int getColorBoxHeight(const bool &sidebar);

};

#endif // BUILDINGDISPLAYUTILS_H
