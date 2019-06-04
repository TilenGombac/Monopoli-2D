#ifndef POSSESSABLEFIELDUTILS_H
#define POSSESSABLEFIELDUTILS_H

#include <QVector>

#include "core/game/action.h"
#include "core/game/entity/fields/possessablefield.h"

class Field;

class PossessableFieldUtils
{
public:
	static float getFieldValue(Field *field);
	static int fieldsInGroup(const QVector<Field*> &fields,
								const int &groupId, const int &ownerId = -1);

	static void setOwnerId(Field *field, const int &playerId);
	static QVector<PossessableField*> getGroupFields(const QVector<Field*> &fields,
														const int &groupId);

	static void setMultiply(const QVector<Field*> &fields, const int &groupId,
								const int &ownerId, const GroupMultiply &multiply);

};

#endif // POSSESSABLEFIELDUTILS_H
