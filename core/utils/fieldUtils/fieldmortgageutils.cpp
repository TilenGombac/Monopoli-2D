#include "fieldmortgageutils.h"

#include "core/game/entity/fields/companyfield.h"
#include "core/game/entity/fields/propertyfield.h"

#include "core/utils/fieldUtils/fieldbuildingutils.h"
#include "core/utils/fieldUtils/possessablefieldutils.h"

bool FieldMortgageUtils::canMortgage(const QVector<Field*> &fields,
									 Field *field, const int &playerId)
{
	FieldType fieldType = field->getFieldType();

	if(fieldType == FieldType::COMPANY_FIELD)
	{
		CompanyField *tmp = dynamic_cast<CompanyField*>(field);

		if((tmp->getOwnerId() == playerId) &&
				(tmp->getMortgaged() == false))
		{
			return true;
		}
	}

	else if(fieldType == FieldType::PROPERTY_FIELD)
	{
		PropertyField *tmp = dynamic_cast<PropertyField*>(field);

		if((tmp->getOwnerId() == playerId)
				&& (tmp->getMortgaged() == false))
		{
			QVector<PossessableField*> groupFields;
			groupFields =
					PossessableFieldUtils::getGroupFields(fields,
														tmp->getGroupId());

			for(PossessableField *f : groupFields)
			{
				if(f->getFieldType() == FieldType::PROPERTY_FIELD)
				{
					tmp = dynamic_cast<PropertyField*> (f);

					if(tmp->getOwnerId() == playerId)
					{
						if((tmp->getBuildings() > PropertyBuildings::HOUSE_0X))
						{
							return false;
						}
					}
				}
			}

			return true;
		}
	}

	return false;
}

bool FieldMortgageUtils::canUnmortgage(Field *field,
										const int &playerId)
{
	if(field->getPossessable())
	{
		PossessableField *tmp = dynamic_cast<PossessableField*> (field);

		if((tmp->getMortgaged()) && (tmp->getOwnerId() == playerId))
		{
			return true;
		}
	}

	return false;
}
