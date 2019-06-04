#include "fieldcopyutils.h"

#include "core/game/entity/fields/cardfield.h"
#include "core/game/entity/fields/companyfield.h"
#include "core/game/entity/fields/cornerfield.h"
#include "core/game/entity/fields/propertyfield.h"
#include "core/game/entity/fields/taxfield.h"

Field* FieldCopyUtils::getCopy(Field *field, QWidget* parent)
{
	FieldType type = field->getFieldType();

	if(type == FieldType::CARD_FIELD)
	{
		CardField *tmp = dynamic_cast<CardField*>(field);

		return new CardField(tmp, parent);
	}
	if(type == FieldType::COMPANY_FIELD)
	{
		CompanyField *tmp = dynamic_cast<CompanyField*>(field);

		return new CompanyField(tmp, parent);
	}
	if(type == FieldType::CORNER_FIELD)
	{
		CornerField *tmp = dynamic_cast<CornerField*>(field);

		return new CornerField(tmp, parent);
	}
	if(type == FieldType::PROPERTY_FIELD)
	{
		PropertyField *tmp = dynamic_cast<PropertyField*>(field);

		return new PropertyField(tmp, parent);
	}

	TaxField *tmp = dynamic_cast<TaxField*>(field);

	return new TaxField(tmp, parent);
}
