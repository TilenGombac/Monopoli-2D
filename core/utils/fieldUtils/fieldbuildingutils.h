#ifndef FIELDBUILDINGUTILS_H
#define FIELDBUILDINGUTILS_H

#include <QVector>

class Field;

class FieldBuildingUtils
{
public:
	static bool canBuild(const QVector<Field*> &fields,
						 Field *field, const int &playerId);
	static bool canDestruct(const QVector<Field*> &fields,
							Field *field, const int &playerId);
	static int getMinBuildingCount(const QVector<Field*> &fields,
								   Field *field);
	static int getMaxBuildingCount(const QVector<Field*> &fields,
									Field *field);

	static bool actionAllowed(const QVector<Field*> &fields,
							 Field *field, const int &playerId,
								const bool &build);

};

#endif // FIELDBUILDINGUTILS_H
