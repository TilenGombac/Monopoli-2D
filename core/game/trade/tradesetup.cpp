#include "tradesetup.h"

#include <QLabel>
#include <QPainter>
#include <QSpinBox>
#include <QToolButton>

#include "core/game/board/board.h"
#include "core/game/trade/offer.h"
#include "core/game/entity/field.h"
#include "core/game/player/player.h"
#include "core/game/trade/playerpicker.h"
#include "core/game/managers/boardmanager.h"
#include "core/utils/windowUtils/windowutils.h"
#include "core/utils/errorUtils/errorhandler.h"
#include "core/game/entity/fields/possessablefield.h"

TradeSetup::TradeSetup(BoardManager *bm, Player *provider, QWidget *parent)
	: QWidget(parent)
{
	this->setFixedSize(400, 300);

	offer = new Offer(provider);

	this->setWindowTitle("Nastavitve menjave");
	this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::Window);

	this->hide();

	picking = CurrentlyPicking::NONE;

	playerPicker = new PlayerPicker(bm->getBoard()->getPlayers(),
										provider->getId(), 0);
	playerPicker->show();

	connect(playerPicker, SIGNAL(playerPicked(Player*)),
				this, SLOT(setRecipient(Player*)));

	providerSpinBox = new QSpinBox(this);
	providerSpinBox->setGeometry(10, 60, this->width() / 3, 30);
	providerSpinBox->setMinimum(0);
	providerSpinBox->setMaximum(provider->getAccountBalance());

	recipientSpinBox = new QSpinBox(this);
	recipientSpinBox->setGeometry(this->width() / 2 + 10, 60,
									this->width() / 3, 30);
	recipientSpinBox->setMinimum(0);
	recipientSpinBox->setDisabled(true);

	connect(providerSpinBox, SIGNAL(valueChanged(int)),
				this, SLOT(changeAmount(int)));

	connect(recipientSpinBox, SIGNAL(valueChanged(int)),
				this, SLOT(changeAmount(int)));

	titleProvider = new QLabel("Ponudba", this);
	titleProvider->setGeometry(0, 0, this->width() / 2, 50);
	titleProvider->setAlignment(Qt::AlignCenter);
	titleProvider->setFont(QFont("Verdana", 14));

	titleRecipient = new QLabel("V zameno za", this);
	titleRecipient->setGeometry(this->width() / 2, 0, this->width() / 2, 50);
	titleRecipient->setAlignment(Qt::AlignCenter);
	titleRecipient->setFont(QFont("Verdana", 14));

	currencyProvider = new QLabel("€", this);
	currencyProvider->setGeometry(this->width() / 3 + 15, 60, 60, 30);
	currencyProvider->setAlignment(Qt::AlignVCenter);

	currencyRecipient = new QLabel("€", this);
	currencyRecipient->setGeometry(5 * this->width() / 6 + 15,
									60, 60, 30);
	currencyRecipient->setAlignment(Qt::AlignVCenter);

	fieldsProvider = new QLabel("Izbira polj ponudbe...", this);
	fieldsProvider->setAlignment(Qt::AlignTop);
	fieldsProvider->setGeometry(10, 100, this->width() / 2 - 20,
									this->height() - 200);

	fieldsRecipient = new QLabel(this);
	fieldsRecipient->setAlignment(Qt::AlignTop);
	fieldsRecipient->setGeometry(this->width() / 2 + 10, 100,
									this->width() / 2 - 20,
										this->height() - 200);

	saveProvider = new QToolButton(this);
	saveProvider->setText("Shrani");
	saveProvider->setGeometry(0, this->height() - 100, this->width() / 2,
								50);

	saveRecipient = new QToolButton(this);
	saveRecipient->setText("Shrani");
	saveRecipient->setGeometry(this->width() / 2, this->height() - 100,
								this->width() / 2, 50);
	saveRecipient->setDisabled(true);

	connect(saveProvider, SIGNAL(clicked()),
				this, SLOT(saveCurrentPicks()));

	connect(saveRecipient, SIGNAL(clicked()),
				this, SLOT(saveCurrentPicks()));

	cancelOfferButton = new QToolButton(this);
	cancelOfferButton->setText("Prekliči");
	cancelOfferButton->setGeometry(0, this->height() - 50,
									this->width() / 2, 50);

	sendOfferButton = new QToolButton(this);
	sendOfferButton->setText("Pošlji ponudbo");
	sendOfferButton->setGeometry(this->width() / 2, this->height() - 50,
									this->width() / 2, 50);
	sendOfferButton->setDisabled(true);

	connect(cancelOfferButton, SIGNAL(clicked()),
				this, SLOT(cancelOffer()));

	connect(sendOfferButton, SIGNAL(clicked()),
				this, SLOT(sendOffer()));

	WindowUtils::centerToScreen(this);
}

TradeSetup::~TradeSetup()
{

}

void TradeSetup::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);

	QPainter painter(this);

	painter.drawLine(this->width() / 2, 0,
						this->width() / 2, this->height());
}

void TradeSetup::addField(Field *selectedField)
{
	OfferPair pair;

	if(picking == CurrentlyPicking::NONE)
	{
		return;
	}

	else if(picking == CurrentlyPicking::PROVIDER)
	{
		if(selectedField->getPossessable())
		{
			pair = offer->getProvided();

			PossessableField *tmp =
					dynamic_cast<PossessableField*> (selectedField);

			if(pair.getFields().size() >= 5)
			{

				// Should let the user remove fields still
				if(pair.getFields().indexOf(tmp) == -1)
				{
					ErrorHandler::message("Največ 5 polj",
											"Za menjavo je dovoljenih največ"
											" 5 polj za ponudbo ter največ"
											" 5 polj za v zameno.");

					return;
				}
			}

			if(tmp->getOwnerId() != offer->getProvider()->getId())
			{
				ErrorHandler::message("To polje ni v tvoji lasti!",
										"Ponudiš lahko le polja v tvoji"
										" lasti");

				return;
			}

			pair.addField(tmp);

			offer->setOfferPairProvided(pair);

			QString fieldsProvided;

			for(PossessableField *f : pair.getFields())
			{
				fieldsProvided += f->getName() + "\n";
			}

			if(fieldsProvided.size() == 0)
			{
				fieldsProvided = "Izbira polj ponudbe...";
			}

			fieldsProvider->setText(fieldsProvided);
		}
	}
	else
	{
		if(selectedField->getPossessable())
		{
			pair = offer->getReturned();

			PossessableField *tmp =
					dynamic_cast<PossessableField*> (selectedField);

			if(pair.getFields().size() >= 5)
			{
				// Should let the user remove fields still
				if(pair.getFields().indexOf(tmp) == -1)
				{
					ErrorHandler::message("Največ 5 polj",
											"Za menjavo je dovoljenih največ"
											" 5 polj za ponudbo ter največ"
											" 5 polj za v zameno.");

					return;
				}
			}

			if(tmp->getOwnerId() != offer->getRecipient()->getId())
			{
				ErrorHandler::message("To polje ni v lasti igralca '"
									   + offer->getRecipient()->getName()
										+ "'",
										"V zameno lahko zahtevaš le polja,"
										" ki jih ta igralec že ima v lasti");

				return;
			}

			pair.addField(tmp);

			offer->setOfferPairReturned(pair);

			QString fieldsReturned;

			for(PossessableField *f : pair.getFields())
			{
				fieldsReturned += f->getName() + "\n";
			}

			if(fieldsReturned.size() == 0)
			{
				fieldsReturned = "Izbira polj za menjavo...";
			}

			fieldsRecipient->setText(fieldsReturned);
		}
	}
}

void TradeSetup::sendOffer()
{
	if(picking == CurrentlyPicking::RECIPIENT)
	{
		offer->getRecipient()->suggestTrade(offer);

		emit offerSent();

		emit offerSent(offer);

		this->hide();

		this->deleteLater();
	}
}

void TradeSetup::changeAmount(int value)
{
	OfferPair pair;

	if(picking == CurrentlyPicking::PROVIDER)
	{
		pair = offer->getProvided();

		pair.changeAmount(value);

		offer->setOfferPairProvided(pair);
	}
	else
	{
		pair = offer->getReturned();

		pair.changeAmount(value);

		offer->setOfferPairReturned(pair);
	}
}

void TradeSetup::setRecipient(Player *player)
{
	picking = CurrentlyPicking::PROVIDER;
	offer->setRecipient(player);
	recipientSpinBox->setMaximum(player->getAccountBalance());

	playerPicker->deleteLater();

	this->show();
}

void TradeSetup::saveCurrentPicks()
{
	if(picking == CurrentlyPicking::PROVIDER)
	{
		picking = CurrentlyPicking::RECIPIENT;

		providerSpinBox->setDisabled(true);

		fieldsRecipient->setText("Izbira polj za menjavo...");

		recipientSpinBox->setDisabled(false);

		saveProvider->setDisabled(true);

		saveRecipient->setDisabled(false);
	}
	else
	{
		recipientSpinBox->setDisabled(true);

		saveRecipient->setDisabled(true);

		sendOfferButton->setDisabled(false);
	}
}

void TradeSetup::cancelOffer()
{
	emit offerCancelled();

	this->hide();

	this->deleteLater();
}
