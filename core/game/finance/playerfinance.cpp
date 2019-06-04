#include "playerfinance.h"

#include "core/utils/ruleUtils/ruleconstantutils.h"

#include "core/utils/errorUtils/errorhandler.h"

#include "core/sounds/soundmanager.h"

PlayerFinance::PlayerFinance()
	: QObject()
{
	balance = RuleConstantUtils::STARTING_BALANCE;
}

bool PlayerFinance::withdraw(const float &amount)
{
	if(balance >= amount)
	{
		SoundManager::play(Sound::WITHDRAW);

		balance -= amount;

		ErrorHandler::log("Withdraw",
							"Amount: " + QString::number(amount));

		emit balanceChanged();

		return true;
	}

	return false;
}

bool PlayerFinance::deposit(const float &amount)
{
	if(amount >= 0)
	{
		SoundManager::play(Sound::DEPOSIT);

		balance += amount;

		ErrorHandler::log("Deposit",
							"Amount: " + QString::number(amount));

		emit balanceChanged();

		return true;
	}

	return false;
}

float PlayerFinance::getAccountBalance()
{
	return balance;
}
