#ifndef PLAYERFINANCE_H
#define PLAYERFINANCE_H

#include <QObject>

class PlayerFinance : public QObject
{
	Q_OBJECT

public:
	PlayerFinance();

	// bool in case there are not enough funds
	bool withdraw(const float &amount);

	// bool in case the amount is negative
	bool deposit(const float &amount);

	float getAccountBalance();

private:
	// Private as the child classes should only touch this via
	// the provided methods
	float balance;

signals:
	void balanceChanged();

};

#endif // PLAYERFINANCE_H
