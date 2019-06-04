#include "boardmovementutils.h"

#include "core/game/entity/fields/cardfield.h"
#include "core/game/entity/fields/companyfield.h"
#include "core/game/entity/fields/cornerfield.h"
#include "core/game/entity/fields/propertyfield.h"
#include "core/game/entity/fields/taxfield.h"

#include "core/game/player/ai.h"
#include "core/game/player/localplayer.h"
#include "core/game/player/networkplayer.h"

#include "core/game/entity/token.h"

int BoardMovementUtils::getTargetId(Player *player, const int &diceValue)
{
	int currentFieldId = player->getToken()->getField()->getId();

	// max field id is 39
	int targetFieldId = (currentFieldId + diceValue) % 40;

	return targetFieldId;
}

bool BoardMovementUtils::move(Player *player, Field *target)
{
	bool passedGo = false;

	if(target->getId() < player->getToken()->getField()->getId())
	{
		// When landing on start, the collect action will take care
		//  of it
		if(target->getId() != 0)
		{
			passedGo = true;
		}
	}

	player->getToken()->move(target);

	return passedGo;
}
