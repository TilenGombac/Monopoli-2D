#include "field.h"

#include <QPainter>
#include <QMouseEvent>

#include "core/utils/fieldUtils/fieldgeometryutils.h"
#include "core/utils/fieldUtils/fieldrotationutils.h"
#include "core/utils/fieldUtils/fieldtranslateutils.h"
#include "core/utils/fieldUtils/fieldDisplayUtils/fielddisplayutils.h"

Field::Field(QWidget *parent)
	: QWidget(parent)
{
	id = 0;

	this->initialize();
}

Field::Field(const int &setId, QWidget *parent)
	: QWidget(parent)
{
	id = setId;

	this->initialize();
}

Field::Field(Field *field, QWidget *parent)
	: QWidget(parent)
{
	id        = field->getId();
	name      = field->getName();
	sidebar   = field->getSidebar();
	fieldType = field->getFieldType();
	action    = field->getAction();
}

void Field::setId(const int &setId)
{
	id = setId;
}

void Field::setName(const QString &setName)
{
	name = setName;
}

void Field::setSidebar(const bool &setSidebar)
{
	sidebar = setSidebar;
}

void Field::setAction(const Action &setAction)
{
	action = setAction;
}

FieldType Field::getFieldType()
{
	return fieldType;
}

int Field::getId()
{
	return id;
}

QString Field::getName()
{
	return name;
}

bool Field::getSidebar()
{
	return sidebar;
}

Action Field::getAction()
{
	return action;
}

bool Field::getPossessable()
{
	if(fieldType == FieldType::PROPERTY_FIELD ||
			fieldType == FieldType::COMPANY_FIELD)
	{
		return true;
	}

	return false;
}

void Field::initialize()
{
	sidebar = false;

	this->setGeometry(FieldGeometryUtils::getRotatedRect(id));
}

void Field::mouseReleaseEvent(QMouseEvent *event)
{
	// No need to emit, if it's only a visual representation in the sidebar
	if((event->button() == Qt::LeftButton) && (sidebar == false))
	{
		this->sendClicked();
	}
}

void Field::sendClicked()
{
	emit fieldSelected(this);
	emit clicked();
}
