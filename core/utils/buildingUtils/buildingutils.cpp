#include "buildingutils.h"

#include "core/game/entity/fields/propertyfield.h"

float BuildingUtils::getBuildingPrice(Field *field)
{
	// Works according to the rules at
	//	http://monopoly.wikia.com/wiki/Houses

	if(field->getFieldType() == FieldType::PROPERTY_FIELD)
	{
		int fieldId = field->getId();

		if((fieldId > 0) && (fieldId < 10))
			return 50;

		if((fieldId > 10) && (fieldId < 20))
			return 100;

		if((fieldId > 20) && (fieldId < 30))
			return 150;

		if((fieldId > 30) && (fieldId < 40))
			return 200;
	}

	return 0;
}
