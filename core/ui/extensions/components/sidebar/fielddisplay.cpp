#include "fielddisplay.h"

#include <QLabel>

#include "core/game/board/board.h"
#include "core/game/entity/field.h"
#include "core/utils/fieldUtils/fieldcopyutils.h"
#include "core/game/entity/fields/companyfield.h"
#include "core/game/entity/fields/propertyfield.h"
#include "core/utils/extensionUtils/fieldDisplay/sidebarfieldutils.h"

FieldDisplay::FieldDisplay(const QVector<Field*> &fields, QWidget *parent)
	: QWidget(parent)
{	
	rentLabel = new QLabel(this);

	originalFields = fields;

	field = nullptr;

	initializeConnections();

	this->setGeometry(SidebarFieldUtils::getGeometry());
	this->show();

	rentLabel->setGeometry(SidebarFieldUtils::getRentGeometry());
	rentLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
	rentLabel->setFont(QFont("Verdana", 12));
	rentLabel->hide();
}

FieldDisplay::~FieldDisplay()
{
	delete field;
	field = nullptr;
}

void FieldDisplay::initializeConnections()
{
	for(Field *tmp : originalFields)
	{
		if(tmp->getId() == 0)
		{
			setDisplayField(tmp);
		}

		this->connectField(tmp);
	}
}

void FieldDisplay::connectField(Field *setField)
{
	connect(setField, SIGNAL(clicked()), this, SLOT(changeField()));
}

void FieldDisplay::setDisplayField(Field *setField)
{
	delete field;
	field = nullptr;

	field = FieldCopyUtils::getCopy(setField, this);

	field->setSidebar(true);

	field->setGeometry(SidebarFieldUtils::getFieldGeometry(field->getId()));
	field->show();

	showRent(setField);
}

void FieldDisplay::showRent(Field *field)
{
	// Only display on possessable fields
	if(field->getPossessable())
	{
		rentLabel->show();

		QString rentText = "Najemnine\n\n";

		if(field->getFieldType() == FieldType::PROPERTY_FIELD)
		{
			PropertyField* tmp = (PropertyField*)field;

			float baseRent     = tmp->getBaseRent();
			float monopolyRent = baseRent * 2;
			float houseRent1X  = baseRent * 5;
			float houseRent2X  = baseRent * 15;
			float houseRent3X  = baseRent * 35;
			float houseRent4X  = baseRent * 45;
			float hotelRent    = baseRent * 55;

			rentText += "Osnova\t        " + QString::number(baseRent) + "€\n";
			rentText += "Monopol\t        " + QString::number(monopolyRent) + "€\n";
			rentText += "1 hiša\t        " + QString::number(houseRent1X) + "€\n";
			rentText += "2 hiši\t        " + QString::number(houseRent2X) + "€\n";
			rentText += "3 hiše\t        " + QString::number(houseRent3X) + "€\n";
			rentText += "4 hiše\t        " + QString::number(houseRent4X) + "€\n";
			rentText += "Hotel\t        " + QString::number(hotelRent) + "€";
		}
		else if(field->getFieldType() == FieldType::COMPANY_FIELD)
		{
			int fieldId = field->getId();

			// Utilities fields
			if((fieldId == 12) || (fieldId == 28))
			{
				rentText += "Osnova\n(vrednost kock x 4) €\n\n";
				rentText += "Monopol\n(vrednost kock x 10) €";
			}
			// Railroads
			else
			{
				float baseRent   = 25;
				float multiply2X = baseRent * 2;
				float multiply3X = baseRent * 4;
				float multiply4X = baseRent * 8;

				rentText += "1 železnica\t" + QString::number(baseRent) + "€\n";
				rentText += "2 železnici\t" + QString::number(multiply2X) + "€\n";
				rentText += "3 železnice\t" + QString::number(multiply3X) + "€\n";
				rentText += "4 železnice\t" + QString::number(multiply4X) + "€\n";
			}
		}

		rentLabel->setText(rentText);
	}
	else
	{
		rentLabel->hide();
	}
}

Field* FieldDisplay::getSelectedField()
{
	// field is only a copy

	for(Field *tmp : originalFields)
	{
		if(tmp->getId() == field->getId())
		{
			return tmp;
		}
	}

	return field;
}

void FieldDisplay::changeField()
{
	Field *sender = qobject_cast<Field*> (QObject::sender());

	setDisplayField(sender);

	emit fieldChanged();
}
