#ifndef ACTION_H
#define ACTION_H

#include <QPair>

enum class ActionType
{
	NO_ACTION = 0,
	COLLECT   = 1,
	MOVE      = 2,
	JAIL      = 3,
	CARD      = 4,
	PAY       = 5
};

class Action
{
	ActionType actionType;
	float actionValue;

public:
	typedef QPair<ActionType, float> ActionPair;

	Action();

	void setNoAction();
	void setCollectAction(const float &value);
	void setMoveAction(const int &fieldId);
	void setGoToJailAction();
	void setGetOutOfJailAction();
	void setCommunityChestAction();
	void setChanceAction();
	void setPayAction(const float &value);

	ActionType getActionType();
	float getActionValue();

	ActionPair getActionPair();

	static const int GO_TO_JAIL      = 0;
	static const int GET_OUT_OF_JAIL = 1;

	static const int COMMUNITY_CHEST = 0;
	static const int CHANCE          = 1;

};

#endif // ACTION_H
