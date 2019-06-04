#ifndef FIELDSMANAGER_H
#define FIELDSMANAGER_H

#include <QVector>

class Field;
class QWidget;
class CornerField;

class FieldsManager
{
public:
	static QVector<Field*> getDefaultFields(QWidget *parent);

};

#endif // FIELDSMANAGER_H
