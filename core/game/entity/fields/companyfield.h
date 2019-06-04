#ifndef COMPANYFIELD_H
#define COMPANYFIELD_H

#include "core/game/entity/fields/possessablefield.h"

class CompanyField : public PossessableField
{
	QImage image;

public:
	CompanyField(QWidget *parent = 0);
	CompanyField(const int &id, QWidget *parent = 0);
	CompanyField(CompanyField *field, QWidget *parent = 0);

	QImage getImage();

	float getRent(const bool &utilities);

	void setImage(const QImage &setImage);

	QByteArray getBytes();

protected:
	void paintEvent(QPaintEvent *event);
};

#endif // COMPANYFIELD_H
