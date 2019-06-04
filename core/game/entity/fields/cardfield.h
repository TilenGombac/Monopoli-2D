#ifndef CARDFIELD_H
#define CARDFIELD_H

#include "core/game/entity/field.h"

class CardField : public Field
{
	QImage image;

public:
	CardField(QWidget *parent = 0);
	CardField(const int &id, QWidget *parent = 0);
	CardField(CardField *field, QWidget *parent = 0);

	QImage getImage();

	void setImage(const QImage &setImage);

	QByteArray getBytes();

protected:
	void paintEvent(QPaintEvent *event);
};

#endif // CARDFIELD_H
