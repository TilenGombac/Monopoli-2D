#ifndef PLAYER_H
#define PLAYER_H

/*
 * This class does not inherit from QWidget as it's sole purpose is
 * to take care of the logic behind player actions, not the graphics.
 * It does however inherit PlayerFinance which inherits QObject, so that
 * signals and slots can be used. For a graphical representation of the
 * player and the player's actions, the token class is used.
 */

#include <QVector>

#include "core/game/finance/playerfinance.h"

class Token;
class Field;
class Offer;
class PossessableField;

enum class PlayerType
{
	NETWORK = 0,
	LOCAL   = 1,
	AI      = 2
};

class Player : public PlayerFinance
{

	Q_OBJECT

public:
	// The token should already be initialized before the player is
	Player(Token *setToken);
	Player(Token *setToken, const QString &setName);
	Player(Token *setToken, const QString &setName, const int &setId);

	virtual ~Player();

	int getId();
	int getJailedTurnsLeft();
	Token* getToken();
	QVector<Field*> getFields();
	QString getName();
	PlayerType getPlayerType();
	bool getJailed();

	void setName(const QString &setName);

	virtual void requestPayment(const float &amount);

	void jailTick();

	void leaveJail(const bool &pay = false);
	void sendToJail();

	float getNetWorth();

	// Buying fails if the player does not have enough funds
	bool purchase(Field *field);

	// Mortgage fails if the field is not ready to be morgaged
	// as in contains houses or hotels
	bool mortgage(Field *field);
	bool unmortgage(Field *field);

	virtual void suggestTrade(Offer *offer);

	bool transfer(PossessableField *field, Player *player);
	bool transfer(const float &amount, Player *player);

	void appendField(Field *field);
	void removeField(Field *field);

private:
	void initialize();

protected:
	int id;

	PlayerType playerType;

	int jailedTurnsLeft;

	bool jailed;

	// The visual representation of the player
	Token *token;

	// The list of fields (properties) the player owns
	QVector<Field*> fields;
	QVector<Field*> mortgagedFields;

	// The name under which the player will be represented
	QString name;

public slots:
	void sendOfferAccepted(Player *recipient);
	void sendOfferDenied(Player *recipient);

signals:
	void paymentRequestFailed(float amount);
	void paymentRequestSuccessful(float amount);
	void leaveGame(Player *player);
	void tradeSuggested(Offer *offer);
	void offerAccepted(Player *recipient);
	void offerDenied(Player *recipient);

};

#endif // PLAYER_H
