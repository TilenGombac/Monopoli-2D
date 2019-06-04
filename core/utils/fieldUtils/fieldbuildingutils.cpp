#include "fieldbuildingutils.h"

#include <QtMath>

#include "core/game/entity/fields/companyfield.h"
#include "core/game/entity/fields/propertyfield.h"

#include "core/utils/fieldUtils/possessablefieldutils.h"

bool FieldBuildingUtils::canBuild(const QVector<Field*> &fields,
								  Field *field, const int &playerId)
{	
	return actionAllowed(fields, field, playerId, true);
}

bool FieldBuildingUtils::canDestruct(const QVector<Field*> &fields,
									 Field *field, const int &playerId)
{
	return actionAllowed(fields, field, playerId, false);
}

int FieldBuildingUtils::getMinBuildingCount(const QVector<Field*> &fields,
												Field *field)
{
	int minCount = 6;
	int groupId  = -1;

	if(field->getFieldType() != FieldType::PROPERTY_FIELD)
	{
		return -1;
	}
	else
	{
		PropertyField *tmp = dynamic_cast<PropertyField*> (field);

		groupId = tmp->getGroupId();
	}

	QVector<PossessableField*> groupFields;
	groupFields = PossessableFieldUtils::getGroupFields(fields, groupId);

	for(PossessableField *groupField : groupFields)
	{
		if(groupField->getFieldType() == FieldType::PROPERTY_FIELD)
		{
			PropertyField *tmp = dynamic_cast<PropertyField*> (groupField);

			int propertyBuildings = (int)tmp->getBuildings();

			if(minCount > propertyBuildings)
			{
				minCount = propertyBuildings;
			}
		}
	}

	return minCount;
}

int FieldBuildingUtils::getMaxBuildingCount(const QVector<Field *> &fields,
												Field *field)
{
	int maxCount = 0;
	int groupId  = -1;

	if(field->getFieldType() != FieldType::PROPERTY_FIELD)
	{
		return -1;
	}
	else
	{
		PropertyField* tmp = dynamic_cast<PropertyField*> (field);

		groupId = tmp->getGroupId();
	}

	QVector<PossessableField*> groupFields;
	groupFields = PossessableFieldUtils::getGroupFields(fields, groupId);

	for(PossessableField* groupField : groupFields)
	{
		if(groupField->getFieldType() == FieldType::PROPERTY_FIELD)
		{
			PropertyField* tmp = dynamic_cast<PropertyField*> (groupField);

			int propertyBuildings = (int)tmp->getBuildings();

			if(maxCount < propertyBuildings)
			{
				maxCount = propertyBuildings;
			}
		}
	}

	return maxCount;
}

bool FieldBuildingUtils::actionAllowed(const QVector<Field *> &fields,
									  Field *field, const int &playerId,
										const bool &build)
{
	PropertyBuildings forbiddenBuildingCase = PropertyBuildings::HOUSE_0X;

	if(build)
	{
		forbiddenBuildingCase = PropertyBuildings::HOTEL;
	}

	if(field->getFieldType() == FieldType::PROPERTY_FIELD)
	{
		for(Field* tmp : fields)
		{
			if(((PropertyField*)tmp)->getMortgaged())
			{
				return false;
			}
		}

		PropertyField *tmp = dynamic_cast<PropertyField*>(field);

		int fieldsInGroup =
				PossessableFieldUtils::fieldsInGroup(fields,
													tmp->getGroupId());

		int ownerFieldsInGroup =
				PossessableFieldUtils::fieldsInGroup(fields,
													 tmp->getGroupId(),
														playerId);

		if((fieldsInGroup == ownerFieldsInGroup)
				&& (tmp->getBuildings() != forbiddenBuildingCase)
					&& (tmp->getOwnerId() == playerId))
		{
			if(build)
			{
				int minCount = getMinBuildingCount(fields, field);

				if((int)tmp->getBuildings() <= minCount)
				{
					return true;
				}
			}
			else
			{
				int maxCount = getMaxBuildingCount(fields, field);

				if((int)tmp->getBuildings() >= maxCount)
				{
					return true;
				}
			}
		}
	}

	return false;
}
