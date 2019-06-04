#ifndef FIELD_H
#define FIELD_H

#include <QWidget>

#include "core/game/action.h"

enum class FieldType
{
	PROPERTY_FIELD = 0,
	COMPANY_FIELD  = 1,
	CORNER_FIELD   = 2,
	CARD_FIELD     = 3,
	TAX_FIELD      = 4
};

class Field : public QWidget
{
	Q_OBJECT

public:
	Field(QWidget *parent = 0);
	Field(const int &setId, QWidget *parent = 0);
	Field(Field* field, QWidget *parent = 0);

	void setId(const int &setId);
	void setName(const QString &setName);
	void setSidebar(const bool &setSidebar);
	void setAction(const Action &setAction);

	FieldType getFieldType();
	int getId();
	QString getName();
	bool getSidebar();
	Action getAction();
	bool getPossessable();

	virtual QByteArray getBytes() = 0;

protected:
	int id;
	QString name;
	FieldType fieldType;
	Action action;
	bool sidebar;

	void initialize();

	void mouseReleaseEvent(QMouseEvent *event);

public slots:
	void sendClicked();

signals:
	void clicked();
	void fieldSelected(Field *selectedField);

};

#endif // FIELD_H
