#include "fieldutils.h"

#include "core/game/entity/fields/cardfield.h"
#include "core/game/entity/fields/companyfield.h"
#include "core/game/entity/fields/cornerfield.h"
#include "core/game/entity/fields/propertyfield.h"
#include "core/game/entity/fields/taxfield.h"

#include "core/utils/buildingUtils/buildingutils.h"

float FieldUtils::getFieldPrice(Field *field)
{
	float price = 0;

	if(field->getPossessable())
	{
		PossessableField *tmp = dynamic_cast<PossessableField*> (field);

		price = tmp->getPrice();
	}

	return price;
}

bool FieldUtils::canPurchase(Field *field)
{
	bool ret = false;

	FieldType fieldType = field->getFieldType();

	if(getFieldPrice(field) != -1)
	{
		if(fieldType == FieldType::COMPANY_FIELD)
		{
			CompanyField *tmp = dynamic_cast<CompanyField*>(field);

			if(tmp->getOwnerId() == -1)
				ret = true;
		}

		else if(fieldType == FieldType::PROPERTY_FIELD)
		{
			PropertyField *tmp = dynamic_cast<PropertyField*>(field);

			if((tmp->getOwnerId() == -1))
				ret = true;
		}
	}

	return ret;
}

float FieldUtils::getFieldValue(Field *field)
{
	// Net field worth

	if(!field->getPossessable())
		return 0;

	float worth = 0;

	if(field->getFieldType() == FieldType::PROPERTY_FIELD)
	{
		PropertyField *tmp = dynamic_cast<PropertyField*> (field);

		worth += tmp->getPrice();

		int numOfBuildings = (int)tmp->getBuildings();

		worth += BuildingUtils::getBuildingPrice(field) * numOfBuildings;
	}

	else if(field->getFieldType() == FieldType::COMPANY_FIELD)
	{
		CompanyField *tmp = dynamic_cast<CompanyField*> (field);

		worth += tmp->getPrice();
	}

	return worth;
}
