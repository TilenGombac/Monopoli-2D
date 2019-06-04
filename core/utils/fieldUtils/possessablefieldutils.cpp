#include "possessablefieldutils.h"

#include "core/game/entity/fields/companyfield.h"
#include "core/game/entity/fields/propertyfield.h"

#include "core/utils/buildingUtils/buildingutils.h"

float PossessableFieldUtils::getFieldValue(Field *field)
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


int PossessableFieldUtils::fieldsInGroup(const QVector<Field *> &fields,
								const int &groupId, const int &ownerId)
{
	int count = 0;

	QVector<PossessableField*> groupFields = getGroupFields(fields, groupId);

	for(PossessableField *field : groupFields)
	{
		if(ownerId != -1)
		{
			if((field->getOwnerId() == ownerId)
					&& !(field->getMortgaged()))
			{
				count++;
			}
		}
		else
		{
			count++;
		}
	}

	return count;
}

void PossessableFieldUtils::setOwnerId(Field *field, const int &playerId)
{
	if(field->getPossessable() == true)
	{
		PossessableField *tmp = dynamic_cast<PossessableField*> (field);

		tmp->setOwnerId(playerId);
	}
}

QVector<PossessableField*>
PossessableFieldUtils::getGroupFields(const QVector<Field *> &fields,
										const int &groupId)
{
	QVector<PossessableField*> ret;

	for(Field *field : fields)
	{
		if(field->getPossessable())
		{
			PossessableField *tmp = dynamic_cast<PossessableField*> (field);

			if(tmp->getGroupId() == groupId)
			{
				ret.append(tmp);
			}
		}
	}

	return ret;
}

void PossessableFieldUtils::setMultiply(const QVector<Field *> &fields,
									const int &groupId,
										const int &ownerId,
											const GroupMultiply &multiply)
{
	QVector<PossessableField*> groupFields = getGroupFields(fields, groupId);

	for(PossessableField *field : groupFields)
	{
		if(field->getOwnerId() == ownerId)
		{
			field->setGroupMultiply(multiply);
		}
	}
}
