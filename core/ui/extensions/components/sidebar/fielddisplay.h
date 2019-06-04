#ifndef FIELDDISPLAY_H
#define FIELDDISPLAY_H

#include <QWidget>

class Field;
class QLabel;

class FieldDisplay : public QWidget
{
	Q_OBJECT

	Field *field;
	QLabel* rentLabel;

	QVector<Field*> originalFields;

public:
	FieldDisplay(const QVector<Field*> &fields, QWidget *parent = 0);
	~FieldDisplay();

	void initializeConnections();
	void connectField(Field *setField);
	void setDisplayField(Field *setField);

	void showRent(Field* field);

	Field *getSelectedField();

public slots:
	void changeField();

signals:
	void fieldChanged();

};

#endif // FIELDDISPLAY_H
