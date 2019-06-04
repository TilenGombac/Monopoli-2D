#include "player.h"

#include <QtMath>

#include "core/utils/playerUtils/playerutils.h"
#include "core/game/entity/fields/cardfield.h"
#include "core/game/entity/fields/companyfield.h"
#include "core/game/entity/fields/cornerfield.h"
#include "core/game/entity/fields/propertyfield.h"
#include "core/game/entity/fields/possessablefield.h"
#include "core/game/entity/fields/taxfield.h"
#include "core/game/entity/tokens/automobiletoken.h"
#include "core/game/entity/tokens/battleshiptoken.h"
#include "core/game/entity/tokens/howitzertoken.h"
#include "core/game/entity/tokens/scottishterriertoken.h"
#include "core/game/entity/tokens/thimbletoken.h"
#include "core/game/entity/tokens/tophattoken.h"
#include "core/utils/fieldUtils/fieldutils.h"
#include "core/utils/fieldUtils/fieldmortgageutils.h"
#include "core/utils/fieldUtils/possessablefieldutils.h"

#include "core/utils/errorUtils/errorhandler.h"

Player::Player(Token *setToken)
	: PlayerFinance()
{
	token = setToken;

	this->initialize();
}

Player::Player(Token *setToken, const QString &setName)
	: PlayerFinance()
{
	token = setToken;

	name = setName;

	this->initialize();
}

Player::Player(Token *setToken, const QString &setName, const int &setId)
{
	token = setToken;

	name = setName;

	id = setId;

	jailed = false;
	jailedTurnsLeft = 0;
}

Player::~Player()
{
	token->deleteLater();
}

int Player::getId()
{
	return id;
}

int Player::getJailedTurnsLeft()
{
	return jailedTurnsLeft;
}

Token* Player::getToken()
{
	return token;
}

QVector<Field*> Player::getFields()
{
	return fields;
}

QString Player::getName()
{
	return name;
}

PlayerType Player::getPlayerType()
{
	return playerType;
}

bool Player::getJailed()
{
	return jailed;
}

void Player::setName(const QString &setName)
{
	name = setName;
}

void Player::requestPayment(const float &amount)
{
	ErrorHandler::log("Request payment", "Payment requested on "
						+ this->getName() + ", amount: "
							+ QString::number(amount));

	if(!this->withdraw(amount))
	{
		emit paymentRequestFailed(amount);
	}
	else
	{
		emit paymentRequestSuccessful(amount);
	}
}

void Player::jailTick()
{
	jailedTurnsLeft--;
}

void Player::leaveJail(const bool &pay)
{
	if(pay)
	{
		this->requestPayment(50);
	}

	jailedTurnsLeft = 0;
	jailed = false;
}

void Player::sendToJail()
{
	jailedTurnsLeft = 3;
	jailed = true;
}

float Player::getNetWorth()
{
	float worth = this->getAccountBalance();

	for(Field *field : fields)
	{
		worth += FieldUtils::getFieldValue(field);
	}

	return worth;
}

bool Player::purchase(Field *field)
{	
	float price = FieldUtils::getFieldPrice(field);

	// Price -1 means field is unpurchasable (corner, card, tax fields)
	if(price != -1)
	{
		// The player has enough funds to purchase this field
		if(withdraw(price))
		{
			fields.append(field);

			PossessableFieldUtils::setOwnerId(field, id);

			return true;
		}
	}

	return false;
}

bool Player::mortgage(Field *field)
{
	if(FieldMortgageUtils::canMortgage(fields, field, id))
	{
		PossessableField *tmp = dynamic_cast<PossessableField*> (field);

		tmp->setMortgaged(true);

		deposit(tmp->getPrice() / 2.0);

		return true;
	}

	return false;
}

bool Player::unmortgage(Field *field)
{
	if(FieldMortgageUtils::canUnmortgage(field, id))
	{
		PossessableField *tmp = dynamic_cast<PossessableField*> (field);

		float liftMortgagePrice = tmp->getPrice() / 2.0;

		// 10% interest
		liftMortgagePrice = qRound(liftMortgagePrice * 1.1);

		if(withdraw(liftMortgagePrice))
		{
			tmp->setMortgaged(false);

			return true;
		}
	}

	return false;
}

void Player::suggestTrade(Offer *offer)
{	
	emit tradeSuggested(offer);
}

bool Player::transfer(PossessableField *field, Player *player)
{
	if(field->getOwnerId() == this->getId())
	{
		PossessableFieldUtils::setOwnerId(field, player->getId());

		Field *tmp = dynamic_cast<Field*> (field);

		fields.removeAll(tmp);

		player->appendField(tmp);

		return true;
	}

	return false;
}

bool Player::transfer(const float &amount, Player *player)
{
	if(withdraw(amount))
	{
		player->deposit(amount);

		return true;
	}

	return false;
}

void Player::appendField(Field *field)
{
	fields.append(field);
}

void Player::removeField(Field *field)
{
	fields.removeAll(field);
}

void Player::initialize()
{
	id = PlayerUtils::getPlayerId();

	jailed = false;
	jailedTurnsLeft = 0;
}

void Player::sendOfferAccepted(Player *recipient)
{
	emit offerAccepted(recipient);
}

void Player::sendOfferDenied(Player *recipient)
{
	emit offerDenied(recipient);
}
