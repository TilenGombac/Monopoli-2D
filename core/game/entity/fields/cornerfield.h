#ifndef CORNERFIELD_H
#define CORNERFIELD_H

#include "core/game/entity/field.h"

class CornerField : public Field
{
	QImage image;

public:
	CornerField(QWidget *parent = 0);
	CornerField(const int &id, QWidget *parent = 0);
	CornerField(CornerField *field, QWidget *parent = 0);

	QImage getImage();

	void setImage(const QImage &setImage);

	QByteArray getBytes();

protected:
	void paintEvent(QPaintEvent *event);
};

#endif // CORNERFIELD_H
