#ifndef FIELDUTILS_H
#define FIELDUTILS_H

#include <QVector>

class Field;

class FieldUtils
{
public:
	static float getFieldPrice(Field *field);

	static void setOwnerId(Field *field, const int &playerId);

	static bool canPurchase(Field *field);

	// Checks that the field is owned by playerId and that the
	// field does not contain any buildings
	static bool canMortgage(Field *field, const int &playerId);

	static bool canBuild(Field *field, const int &playerId);
	static bool canDestruct(Field *field, const int &playerId);

	static float getFieldValue(Field *field);

	static int fieldsInGroup(const QVector<Field*> &fields,
								const int &groupId,
									const int &ownerId = -1);

};

#endif // FIELDUTILS_H
