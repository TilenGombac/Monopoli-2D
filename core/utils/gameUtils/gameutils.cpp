#include "gameutils.h"

#include "core/game/board/board.h"
#include "core/game/entity/field.h"
#include "core/game/player/player.h"
#include "core/game/managers/boardmanager.h"
#include "core/game/entity/fields/propertyfield.h"
#include "core/utils/buildingUtils/buildingutils.h"
#include "core/utils/fieldUtils/possessablefieldutils.h"

void GameUtils::handlePassGo(Player *player, const bool &passedGo)
{
	if(passedGo)
	{
		player->deposit(200.0);
	}
}

void GameUtils::build(Field *field, Player *player)
{
	if(field->getFieldType() == FieldType::PROPERTY_FIELD)
	{
		PropertyField *tmp = dynamic_cast<PropertyField*> (field);

		if(player->withdraw(BuildingUtils::getBuildingPrice(tmp)))
		{
			addBuilding(field);
		}
	}
}

void GameUtils::destruct(Field *field, Player *player)
{
	if(field->getFieldType() == FieldType::PROPERTY_FIELD)
	{
		PropertyField *tmp = dynamic_cast<PropertyField*> (field);

		float price = BuildingUtils::getBuildingPrice(tmp) / 2.0;

		removeBuilding(field);

		player->deposit(price);
	}
}

void GameUtils::addBuilding(Field *field)
{
	PropertyField *tmp = dynamic_cast<PropertyField*> (field);

	int propertyBuildings = (int)tmp->getBuildings() + 1;

	tmp->setBuildings((PropertyBuildings)propertyBuildings);
}

void GameUtils::removeBuilding(Field *field)
{
	PropertyField *tmp = dynamic_cast<PropertyField*> (field);

	int propertyBuildings = (int)tmp->getBuildings() - 1;

	tmp->setBuildings((PropertyBuildings)propertyBuildings);
}
