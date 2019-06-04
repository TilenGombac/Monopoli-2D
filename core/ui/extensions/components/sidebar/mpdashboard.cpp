#include "mpdashboard.h"

#include "core/game/board/board.h"
#include "core/game/entity/field.h"
#include "core/game/entity/token.h"
#include "core/game/player/player.h"
#include "core/game/trade/tradeview.h"
#include "core/game/trade/tradesetup.h"
#include "core/game/managers/boardmanager.h"
#include "core/utils/fieldUtils/fieldutils.h"
#include "core/utils/errorUtils/errorhandler.h"
#include "core/notifications/notificationmanager.h"
#include "core/utils/fieldUtils/fieldbuildingutils.h"
#include "core/utils/fieldUtils/fieldmortgageutils.h"
#include "core/ui/extensions/components/sidebar/fielddisplay.h"

#include "network/client/localclient.h"

#include "network/packets/buypacket.h"
#include "network/packets/jailpacket.h"
#include "network/packets/endturnpacket.h"
#include "network/packets/rolldicepacket.h"
#include "network/packets/disconnectpacket.h"
#include "network/packets/playeractionpacket.h"

MPDashboard::MPDashboard(BoardManager *bm, Player *localPlayer,
							FieldDisplay *fieldDisplay, LocalClient *client,
								QWidget *sidebar)
	: QWidget(sidebar)
{
	this->client       = client;
	this->player       = localPlayer;
	this->boardManager = bm;
	this->fieldDisplay = fieldDisplay;

	// Available at the beggining, before the first update packet will arrive
	rollDiceAvailable = true;

	// Like the rollDiceAvailable, this one is disabled at first
	endTurnAvailable = false;

	int h = 3 * 60;
	int y = sidebar->height() - fieldDisplay->height() - h;

	this->setGeometry(0, y, sidebar->width(), h);

	this->initializeButtons();
	this->updateDashboard();

	connect(bm->getBoard(), SIGNAL(playerTurnChanged()),
				this, SLOT(updateDashboard()));

	connect(player->getToken(), SIGNAL(reachedTarget()),
				this, SLOT(updateDashboard()));

	connect(fieldDisplay, SIGNAL(fieldChanged()),
				this, SLOT(updateDashboard()));

	connect(client, SIGNAL(updateDashboard(QVector<PlayerAction>)),
				this, SLOT(onUpdateDashboardReceived(QVector<PlayerAction>)));

	connect(client, SIGNAL(paymentFailed(int,float)),
				this, SLOT(onPaymentFailedReceived(int,float)));

	connect(client, SIGNAL(tradeOffered(TradePacket)),
				this, SLOT(onTradeOffered(TradePacket)));

	connect(client, SIGNAL(tradeDenied(TradePacket)),
				this, SLOT(onTradeDenied(TradePacket)));

	connect(client, SIGNAL(tradeAccepted(TradePacket)),
				this, SLOT(onTradeAccepted(TradePacket)));

	this->show();
}

void MPDashboard::initializeButtons()
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

	buttons[(int)PlayerAction::BUY] = new QToolButton(this);
	buttons[(int)PlayerAction::BUY]->setText("Kupi polje");
	buttons[(int)PlayerAction::BUY]->setGeometry(w, 0, w, h);

	buttons[(int)PlayerAction::DESTRUCT] = new QToolButton(this);
	buttons[(int)PlayerAction::DESTRUCT]->setText("Poruši\nzgradbo");
	buttons[(int)PlayerAction::DESTRUCT]->setGeometry(w*2, h, w, h);

	buttons[(int)PlayerAction::BUILD] = new QToolButton(this);
	buttons[(int)PlayerAction::BUILD]->setText("Postavi\nzgradbo");
	buttons[(int)PlayerAction::BUILD]->setGeometry(w*2, 0, w, h);

	buttons[(int)PlayerAction::TRADE] = new QToolButton(this);
	buttons[(int)PlayerAction::TRADE]->setText("Predlagaj\nmenjavo");
	buttons[(int)PlayerAction::TRADE]->setGeometry(w, h, w, h);

	buttons[(int)PlayerAction::LEAVE_JAIL] = new QToolButton(this);
	buttons[(int)PlayerAction::LEAVE_JAIL]->setText("Zapusti\nzapor");
	buttons[(int)PlayerAction::LEAVE_JAIL]->setGeometry(0, h*2, w, h);
	buttons[(int)PlayerAction::LEAVE_JAIL]->setHidden(true);

	connect(buttons[(int)PlayerAction::ROLL_DICE], SIGNAL(clicked()),
				this, SLOT(rollDice()));

	connect(buttons[(int)PlayerAction::END_TURN], SIGNAL(clicked()),
				this, SLOT(endTurn()));

	connect(buttons[(int)PlayerAction::MORTGAGE], SIGNAL(clicked()),
				this, SLOT(mortgage()));

	connect(buttons[(int)PlayerAction::UNMORTGAGE], SIGNAL(clicked()),
				this, SLOT(unmortgage()));

	connect(buttons[(int)PlayerAction::BUILD], SIGNAL(clicked()),
				this, SLOT(build()));

	connect(buttons[(int)PlayerAction::DESTRUCT], SIGNAL(clicked()),
				this, SLOT(destruct()));

	connect(buttons[(int)PlayerAction::BUY], SIGNAL(clicked()),
				this, SLOT(buy()));

	connect(buttons[(int)PlayerAction::TRADE], SIGNAL(clicked()),
				this, SLOT(startTradeSetup()));

	connect(buttons[(int)PlayerAction::LEAVE_JAIL], SIGNAL(clicked()),
				this, SLOT(leaveJail()));

	for(QToolButton *button : buttons)
	{
		connect(button, SIGNAL(clicked()), this, SLOT(updateDashboard()));

		button->setFont(QFont("Verdana", 11));
	}
}

void MPDashboard::updateDashboard()
{
	int playerTurn = boardManager->getBoard()->getPlayerTurn();

	for(QToolButton* button : buttons)
	{
		button->setDisabled(true);
	}

	buttons[(int)PlayerAction::LEAVE_JAIL]->setHidden(true);

	for(Player* p : boardManager->getBoard()->getPlayers())
	{
		// Actions disabled while any player is moving!
		if(p->getToken()->getMoving())
		{
			return;
		}
	}

	// When pending payment, the server does not send
	// rollDice or endTurn actions available and we set them as false
	// and can guarantee that there is payment pending
	bool pendingPayment = !(rollDiceAvailable | endTurnAvailable);

	// Actions not allowed while pending payment:
	//	- Roll dice
	//  - End turn
	//  - Build
	//  - Unmortgage
	//  - Leave jail
	// New action available: goBankrupt (instead of the rollDice Button)

	if(playerTurn == player->getId())
	{
		Field* field = fieldDisplay->getSelectedField();

		if(pendingPayment)
		{
			buttons[(int)PlayerAction::ROLL_DICE]->setDisabled(false);
			buttons[(int)PlayerAction::ROLL_DICE]->setText("Osebni\nstečaj");

			// The bankruptcy action is handled in the rollDice method
		}

		if(rollDiceAvailable)
		{
			buttons[(int)PlayerAction::ROLL_DICE]->setDisabled(false);

			// Reset if pendingPayment earlier
			buttons[(int)PlayerAction::ROLL_DICE]->setText("Vrzi kocki");
		}

		if(endTurnAvailable)
		{
			buttons[(int)PlayerAction::END_TURN]->setDisabled(false);
		}

		if(FieldBuildingUtils::canBuild(boardManager->getBoard()->getFields(),
											field, playerTurn))
		{
			if(!pendingPayment)
			{
				buttons[(int)PlayerAction::BUILD]->setDisabled(false);
			}
		}

		buttons[(int)PlayerAction::DESTRUCT]->setDisabled(false);

		if(FieldMortgageUtils::canMortgage(boardManager->getBoard()->getFields(),
											field, playerTurn))
		{
			buttons[(int)PlayerAction::MORTGAGE]->setDisabled(false);
		}

		if(FieldMortgageUtils::canUnmortgage(field, playerTurn))
		{
			if(!pendingPayment)
			{
				buttons[(int)PlayerAction::UNMORTGAGE]->setDisabled(false);
			}
		}

		// Can only purchase fields we land on
		if(FieldUtils::canPurchase(player->getToken()->getField()))
		{
			if(!pendingPayment)
			{
				buttons[(int)PlayerAction::BUY]->setDisabled(false);
			}
		}

		buttons[(int)PlayerAction::TRADE]->setDisabled(false);

		if(player->getJailed())
		{
			if(!pendingPayment)
			{
				buttons[(int)PlayerAction::LEAVE_JAIL]->setDisabled(false);
				buttons[(int)PlayerAction::LEAVE_JAIL]->setHidden(false);
			}
		}
	}
}

void MPDashboard::onUpdateDashboardReceived(QVector<PlayerAction> actions)
{
	rollDiceAvailable = false;
	endTurnAvailable  = false;

	for(PlayerAction action : actions)
	{
		if(action == PlayerAction::ROLL_DICE)
		{
			rollDiceAvailable = true;
		}
		else
		{
			endTurnAvailable  = true;
		}
	}

	this->updateDashboard();
}

void MPDashboard::onPaymentFailedReceived(int playerId, float amount)
{
	if(player->getId() == playerId)
	{
		ErrorHandler::message("Napaka pri plačilu",
								"Dolguješ " + QString::number(amount)
									+ "€, vendar nimaš dovolj"
										+ " razpoložljivih sredstev.");
	}
}

void MPDashboard::rollDice()
{
	// When pending payment, the server does not send
	// rollDice or endTurn actions available and we set them as false
	// and can guarantee that there is payment pending
	bool pendingPayment = !(rollDiceAvailable | endTurnAvailable);

	if(pendingPayment)
	{
		DisconnectPacket disconnectPacket(this->player->getId());

		this->client->getSocket()->write(disconnectPacket.getData(true));

		return;
	}

	RollDicePacket packet(this->player->getId());

	this->client->getSocket()->write(packet.getData(true));
}

void MPDashboard::endTurn()
{
	EndTurnPacket packet(this->player->getId());

	this->client->getSocket()->write(packet.getData(true));
}

void MPDashboard::buy()
{
	BuyPacket packet(this->player->getId());

	this->client->getSocket()->write(packet.getData(true));
}

void MPDashboard::trade()
{

}

void MPDashboard::mortgage()
{
	PlayerActionPacket packet(this->player->getId(),
								fieldDisplay->getSelectedField()->getId(),
									MP_PlayerAction::MORTGAGE);

	this->client->getSocket()->write(packet.getData(true));
}

void MPDashboard::unmortgage()
{
	PlayerActionPacket packet(this->player->getId(),
								fieldDisplay->getSelectedField()->getId(),
									MP_PlayerAction::UNMORTGAGE);

	this->client->getSocket()->write(packet.getData(true));
}

void MPDashboard::build()
{
	PlayerActionPacket packet(this->player->getId(),
								fieldDisplay->getSelectedField()->getId(),
									MP_PlayerAction::BUILD);

	this->client->getSocket()->write(packet.getData(true));
}

void MPDashboard::destruct()
{
	PlayerActionPacket packet(this->player->getId(),
								fieldDisplay->getSelectedField()->getId(),
									MP_PlayerAction::DESTRUCT);

	this->client->getSocket()->write(packet.getData(true));
}

void MPDashboard::leaveJail()
{
	JailPacket packet(this->player->getId(), JailAction::LEAVE_JAIL);

	this->client->getSocket()->write(packet.getData(true));
}

void MPDashboard::startTradeSetup()
{
	TradeSetup *setup = new TradeSetup(this->boardManager, this->player);

	connect(setup, SIGNAL(offerCancelled()),
				this, SLOT(stopTrade()));

	connect(setup, SIGNAL(offerSent(Offer*)),
				this, SLOT(sendOffer(Offer*)));

	connect(this, SIGNAL(destroyed(QObject*)),
				setup, SLOT(deleteLater()));

	for(Field* f : boardManager->getBoard()->getFields())
	{
		if(f->getPossessable())
		{
			connect(f, SIGNAL(fieldSelected(Field*)),
						setup, SLOT(addField(Field*)));
		}
	}

	setup->hide();

	this->updateDashboard();
}

void MPDashboard::onTradeOffered(TradePacket packet)
{
	if(packet.getRecipientId() == player->getId())
	{
		int providerId = packet.getProviderId();

		QString provider = boardManager->getPlayer(providerId)->getName();

		NotificationManager::pushNotification("Nova ponudba",
												provider + " vam je poslal novo ponudbo.",
													NotificationType::INFORMATION);

		TradeView* tradeView = new TradeView(provider,
											boardManager->getBoard()->getFields(),
												packet, this);

		connect(tradeView, SIGNAL(denied(TradePacket)),
					this, SLOT(denyOffer(TradePacket)));

		connect(tradeView, SIGNAL(accepted(TradePacket)),
					this, SLOT(acceptOffer(TradePacket)));
	}

	this->updateDashboard();
}

void MPDashboard::onTradeDenied(TradePacket packet)
{
	if(packet.getProviderId() == player->getId())
	{
		NotificationManager::pushNotification("Ponudba zavrnjena",
												"Igralec je vašo ponudbo zavrnil.",
													NotificationType::WARNING);
	}

	this->updateDashboard();
}

void MPDashboard::onTradeAccepted(TradePacket packet)
{
	if(packet.getProviderId() == player->getId())
	{
		NotificationManager::pushNotification("Ponudba sprejeta",
												"Igralec je vašo ponudbo sprejel.",
													NotificationType::INFORMATION);
	}

	int providerId  = packet.getProviderId();
	int recipientId = packet.getRecipientId();

	Player* provider  = boardManager->getPlayer(providerId);
	Player* recipient = boardManager->getPlayer(recipientId);

	if((provider != nullptr) && (recipient != nullptr))
	{
		Offer* offer = new Offer(provider);

		offer->setRecipient(recipient);

		QVector<Field*> fields = boardManager->getBoard()->getFields();

		OfferPair provided = packet.getProvided(fields);
		OfferPair returned = packet.getReturned(fields);

		offer->setOfferPairProvided(provided);
		offer->setOfferPairReturned(returned);

		offer->acceptOffer();
	}

	this->updateDashboard();
}

void MPDashboard::denyOffer(TradePacket packet)
{
	packet.setStatus(TradeStatus::DENIED);

	this->client->getSocket()->write(packet.getData(true));
}

void MPDashboard::acceptOffer(TradePacket packet)
{
	packet.setStatus(TradeStatus::ACCEPTED);

	this->client->getSocket()->write(packet.getData(true));
}

void MPDashboard::sendOffer(Offer *offer)
{
	TradePacket packet(offer, TradeStatus::PENDING);

	this->client->getSocket()->write(packet.getData(true));

	NotificationManager::pushNotification("Ponudba poslana",
											"Ponudba je bila uspešno poslana.",
												NotificationType::INFORMATION);
}

void MPDashboard::stopTrade()
{
	this->updateDashboard();
}
