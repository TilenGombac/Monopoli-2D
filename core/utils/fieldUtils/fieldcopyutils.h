#ifndef FIELDCOPYUTILS_H
#define FIELDCOPYUTILS_H

class Field;
class QWidget;

class FieldCopyUtils
{
public:
	static Field* getCopy(Field* field, QWidget *parent = 0);
};

#endif // FIELDCOPYUTILS_H
