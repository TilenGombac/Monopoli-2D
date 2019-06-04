#include "playerdashboard.h"

#include "core/game/board/board.h"
#include "core/game/entity/dice.h"
#include "core/game/entity/field.h"
#include "core/game/entity/token.h"
#include "core/game/player/player.h"
#include "core/game/managers/boardmanager.h"
#include "core/utils/fieldUtils/fieldutils.h"
#include "core/utils/errorUtils/errorhandler.h"
#include "core/game/entity/fields/propertyfield.h"
#include "core/notifications/notificationmanager.h"
#include "core/utils/fieldUtils/fieldbuildingutils.h"
#include "core/utils/fieldUtils/fieldmortgageutils.h"
#include "core/utils/fieldUtils/possessablefieldutils.h"
#include "core/ui/extensions/components/sidebar/fielddisplay.h"

PlayerDashboard::PlayerDashboard(BoardManager *manager, Player *setPlayer,
									FieldDisplay *setFieldDisplay,
										QWidget *sidebar)
	: QWidget(sidebar)
{
	player       = setPlayer;
	boardManager = manager;
	fieldDisplay = setFieldDisplay;

	actionManager = new PlayerActionManager(boardManager, player);

	paymentOnHold = false;

	int h = 3 * 60;
	int y = sidebar->height() - fieldDisplay->height() - h;

	this->setGeometry(0, y, sidebar->width(), h);

	this->initializeButtons();

	connect(actionManager, SIGNAL(actionPerformed()),
				this, SLOT(updateDashboard()));

	connect(player, SIGNAL(paymentRequestFailed(float)),
				this, SLOT(handlePaymentFailed(float)));

	connect(player->getToken(), SIGNAL(reachedTarget()),
				this, SLOT(updateDashboard()));

	connect(fieldDisplay, SIGNAL(fieldChanged()),
				this, SLOT(updateSelectedField()));

	connect(player, SIGNAL(offerAccepted(Player*)),
				this, SLOT(handleAcceptedOffer(Player*)));

	connect(player, SIGNAL(offerDenied(Player*)),
				this, SLOT(handleDeniedOffer(Player*)));

	this->updateDashboard();
}

void PlayerDashboard::initializeButtons()
{
	int w = this->width() / 4;
	int h = this->height() / 3;

	buttons[(int)PlayerAction::UNMORTGAGE] = new QToolButton(this);
	buttons[(int)PlayerAction::UNMORTGAGE]->setText("Odplačaj\nkredit");
	buttons[(int)PlayerAction::UNMORTGAGE]->setGeometry(w*3, h, w, h);

	buttons[(int)PlayerAction::ROLL_DICE] = new QToolButton(this);
	buttons[(int)PlayerAction::ROLL_DICE]->setText("Vrzi kocki");
	buttons[(int)PlayerAction::ROLL_DICE]->setGeometry(0, 0, w, h);

	buttons[(int)PlayerAction::MORTGAGE] = new QToolButton(this);
	buttons[(int)PlayerAction::MORTGAGE]->setText("Vzemi\nkredit");
	buttons[(int)PlayerAction::MORTGAGE]->setGeometry(w*3, 0, w, h);

	buttons[(int)PlayerAction::END_TURN] = new QToolButton(this);
	buttons[(int)PlayerAction::END_TURN]->setText("Zaključi\npotezo");
	buttons[(int)PlayerAction::END_TURN]->setGeometry(0, h, w, h);

	buttons[(int)PlayerAction::DESTRUCT] = new QToolButton(this);
	buttons[(int)PlayerAction::DESTRUCT]->setText("Poruši\nzgradbo");
	buttons[(int)PlayerAction::DESTRUCT]->setGeometry(w*2, h, w, h);

	buttons[(int)PlayerAction::BUILD] = new QToolButton(this);
	buttons[(int)PlayerAction::BUILD]->setText("Postavi\nzgradbo");
	buttons[(int)PlayerAction::BUILD]->setGeometry(w*2, 0, w, h);

	buttons[(int)PlayerAction::TRADE] = new QToolButton(this);
	buttons[(int)PlayerAction::TRADE]->setText("Predlagaj\nmenjavo");
	buttons[(int)PlayerAction::TRADE]->setGeometry(w, h, w, h);

	buttons[(int)PlayerAction::BUY] = new QToolButton(this);
	buttons[(int)PlayerAction::BUY]->setText("Kupi polje");
	buttons[(int)PlayerAction::BUY]->setGeometry(w, 0, w, h);

	buttons[(int)PlayerAction::LEAVE_JAIL] = new QToolButton(this);
	buttons[(int)PlayerAction::LEAVE_JAIL]->setText("Zapusti\nzapor");
	buttons[(int)PlayerAction::LEAVE_JAIL]->setGeometry(0, h*2, w, h);
	buttons[(int)PlayerAction::LEAVE_JAIL]->setHidden(true);

	connect(buttons[(int)PlayerAction::ROLL_DICE], SIGNAL(clicked()),
				actionManager, SLOT(rollDice()));

	connect(buttons[(int)PlayerAction::END_TURN], SIGNAL(clicked()),
				actionManager, SLOT(endTurn()));

	connect(buttons[(int)PlayerAction::MORTGAGE], SIGNAL(clicked()),
				actionManager, SLOT(mortgageField()));

	connect(buttons[(int)PlayerAction::UNMORTGAGE], SIGNAL(clicked()),
				actionManager, SLOT(unmortgageField()));

	connect(buttons[(int)PlayerAction::BUILD], SIGNAL(clicked()),
				actionManager, SLOT(build()));

	connect(buttons[(int)PlayerAction::DESTRUCT], SIGNAL(clicked()),
				actionManager, SLOT(destruct()));

	connect(buttons[(int)PlayerAction::BUY], SIGNAL(clicked()),
				actionManager, SLOT(buyField()));

	connect(buttons[(int)PlayerAction::TRADE], SIGNAL(clicked()),
				actionManager, SLOT(startTradeSetup()));

	connect(buttons[(int)PlayerAction::LEAVE_JAIL], SIGNAL(clicked()),
				this, SLOT(leaveJail()));

	for(QToolButton *button : buttons)
	{
		connect(button, SIGNAL(clicked()), this, SLOT(updateDashboard()));

		button->setFont(QFont("Verdana", 11));
	}
}

void PlayerDashboard::updateDashboard()
{
	if(paymentOnHold)
	{
		return;
	}

	for(QToolButton *button : buttons)
	{
		button->setDisabled(true);
	}

	if((boardManager->getBoard()->getPlayerTurn() == player->getId())
			&& (player->getToken()->getMoving() == false))
	{
		Field *field = fieldDisplay->getSelectedField();

		if(!actionManager->getTradeInProgress())
		{
			buttons[(int)PlayerAction::TRADE]->setDisabled(false);
		}

		if(actionManager->getDiceThrown())
		{
			buttons[(int)PlayerAction::END_TURN]->setDisabled(false);
		}
		else
		{
			buttons[(int)PlayerAction::ROLL_DICE]->setDisabled(false);
		}

		if(FieldBuildingUtils::canBuild(boardManager->getBoard()->getFields(),
											field, player->getId()))
		{
			buttons[(int)PlayerAction::BUILD]->setDisabled(false);
		}

		if(FieldMortgageUtils::canMortgage(boardManager->getBoard()->getFields(),
											field, player->getId()))
		{
			buttons[(int)PlayerAction::MORTGAGE]->setDisabled(false);
		}

		if(FieldMortgageUtils::canUnmortgage(field, player->getId()))
		{
			buttons[(int)PlayerAction::UNMORTGAGE]->setDisabled(false);
		}

		if(FieldBuildingUtils::canDestruct(boardManager->getBoard()->getFields(),
											field, player->getId()))
		{
			buttons[(int)PlayerAction::DESTRUCT]->setDisabled(false);
		}

		// Can only purchase fields we land on
		if(FieldUtils::canPurchase(player->getToken()->getField()))
		{
			buttons[(int)PlayerAction::BUY]->setDisabled(false);
		}

		if(player->getJailed())
		{
			buttons[(int)PlayerAction::LEAVE_JAIL]->setDisabled(false);
			buttons[(int)PlayerAction::LEAVE_JAIL]->setHidden(false);
		}
	}
}

void PlayerDashboard::handlePaymentFailed(float amount)
{
	paymentOnHold = true;

	debt = amount;

	buttons[(int)PlayerAction::END_TURN]->setText("Osebni\nstečaj");

	disconnect(buttons[(int)PlayerAction::END_TURN], SIGNAL(clicked()),
				actionManager, SLOT(endTurn()));

	connect(buttons[(int)PlayerAction::END_TURN], SIGNAL(clicked()),
				this, SLOT(goBankrupt()));

	buttons[(int)PlayerAction::ROLL_DICE]->setText("Poskusi\nodplačati");

	disconnect(buttons[(int)PlayerAction::ROLL_DICE], SIGNAL(clicked()),
				actionManager, SLOT(rollDice()));

	connect(buttons[(int)PlayerAction::ROLL_DICE], SIGNAL(clicked()),
				this, SLOT(attemptPayment()));

	buttons[(int)PlayerAction::END_TURN]->setDisabled(false);
	buttons[(int)PlayerAction::ROLL_DICE]->setDisabled(false);
}

void PlayerDashboard::handlePaymentSuccess()
{
	paymentOnHold = false;

	debt = 0;

	buttons[(int)PlayerAction::END_TURN]->setText("Zaključi\npotezo");

	disconnect(buttons[(int)PlayerAction::END_TURN], SIGNAL(clicked()),
				this, SLOT(goBankrupt()));

	buttons[(int)PlayerAction::ROLL_DICE]->setText("Vrzi kocki");

	disconnect(buttons[(int)PlayerAction::ROLL_DICE], SIGNAL(clicked()),
				this, SLOT(attemptPayment()));

	connect(buttons[(int)PlayerAction::ROLL_DICE], SIGNAL(clicked()),
				actionManager, SLOT(rollDice()));

	this->updateDashboard();
}

void PlayerDashboard::updateSelectedField()
{
	actionManager->setSelectedField(fieldDisplay->getSelectedField());

	this->updateDashboard();
}

void PlayerDashboard::handleAcceptedOffer(Player *recipient)
{
	NotificationManager::pushNotification("Ponudba sprejeta",
										  "Igralec '" + recipient->getName() +
										  "' je vašo ponudbo sprejel.",
											NotificationType::INFORMATION);

}

void PlayerDashboard::handleDeniedOffer(Player *recipient)
{
	ErrorHandler::message("Ponudba zavrnjena",
						  "Igralec '" + recipient->getName() +
						  "' je vašo ponudbo zavrnil.");
}

void PlayerDashboard::goBankrupt()
{
	Field *currentField = player->getToken()->getField();

	// Probably standing on a possessablefield and are in trouble due
	//	to rent cost
	if(currentField->getPossessable())
	{
		PossessableField* tmp = dynamic_cast<PossessableField*> (currentField);

		int ownerId = tmp->getOwnerId();

		if((ownerId != -1) && (ownerId != player->getId()))
		{
			Player* creditor = boardManager->getPlayer(ownerId);

			float funds = player->getAccountBalance();

			debt -= funds;

			player->transfer(funds, creditor);

			for(Field* f : player->getFields())
			{
				debt -= FieldUtils::getFieldValue(f);

				player->transfer((PossessableField*)f, creditor);

				if(debt <= 0)
				{
					break;
				}
			}
		}
	}

	for(Field* f : player->getFields())
	{
		if(f->getFieldType() == FieldType::PROPERTY_FIELD)
		{
			PropertyField* prop = dynamic_cast<PropertyField*> (f);

			prop->setBuildings(PropertyBuildings::HOUSE_0X);

			player->removeField(f);

			prop->setOwnerId(-1);
		}
	}

	boardManager->removePlayer(player);

	this->hide();
}

void PlayerDashboard::attemptPayment()
{
	if(player->withdraw(debt))
	{
		this->handlePaymentSuccess();
	}
	else
	{
		ErrorHandler::message("Napaka",
								"Nimaš dovolj sredstev!");
	}
}

void PlayerDashboard::leaveJail()
{
	if(player->withdraw(50))
	{
		player->leaveJail();

		buttons[(int)PlayerAction::LEAVE_JAIL]->setHidden(true);
	}
	else
	{
		ErrorHandler::message("Napaka",
								"Nimaš dovolj sredstev!");
	}
}
