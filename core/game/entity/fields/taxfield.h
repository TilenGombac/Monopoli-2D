#ifndef TAXFIELD_H
#define TAXFIELD_H

#include "core/game/entity/field.h"

class TaxField : public Field
{
	QString payString; // displayed as {payString} {cost}
	QImage image;
	float cost; // the amount a player has to pay upon landing

public:
	TaxField(QWidget *parent = 0);
	TaxField(const int &id, QWidget *parent = 0);
	TaxField(TaxField *field, QWidget *parent = 0);

	QString getPayString();
	QImage getImage();
	float getCost();

	void setPayString(const QString &setPayString);
	void setImage(const QImage &setImage);
	void setCost(const float &setCost);

	QByteArray getBytes();

protected:
	void paintEvent(QPaintEvent *event);

};

#endif // TAXFIELD_H
