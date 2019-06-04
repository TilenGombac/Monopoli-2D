#include "action.h"

Action::Action()
{
	this->setNoAction();
}

void Action::setNoAction()
{
	actionType  = ActionType::NO_ACTION;
	actionValue = 0;
}

void Action::setCollectAction(const float &value)
{
	actionType  = ActionType::COLLECT;
	actionValue = value;
}

void Action::setMoveAction(const int &fieldId)
{
	actionType  = ActionType::MOVE;
	actionValue = (float)fieldId;
}

void Action::setGoToJailAction()
{
	actionType  = ActionType::JAIL;
	actionValue = (float)GO_TO_JAIL;
}

void Action::setGetOutOfJailAction()
{
	actionType  = ActionType::JAIL;
	actionValue = (float)GET_OUT_OF_JAIL;
}

void Action::setCommunityChestAction()
{
	actionType  = ActionType::CARD;
	actionValue = (float)COMMUNITY_CHEST;
}

void Action::setChanceAction()
{
	actionType  = ActionType::CARD;
	actionValue = (float)CHANCE;
}

void Action::setPayAction(const float &value)
{
	actionType  = ActionType::PAY;
	actionValue = value;
}

ActionType Action::getActionType()
{
	return actionType;
}

float Action::getActionValue()
{
	return actionValue;
}

Action::ActionPair Action::getActionPair()
{
	return QPair<ActionType, float> (actionType, actionValue);
}
