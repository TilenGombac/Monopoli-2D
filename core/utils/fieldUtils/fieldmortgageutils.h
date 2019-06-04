#ifndef FIELDMORTGAGEUTILS_H
#define FIELDMORTGAGEUTILS_H

#include <QVector>

class Field;

class FieldMortgageUtils
{
public:
	static bool canMortgage(const QVector<Field*> &fields,
								Field *field, const int &playerId);
	static bool canUnmortgage(Field *field, const int &playerId);

};

#endif // FIELDMORTGAGEUTILS_H
