#include "cardsmanager.h"

#include <QTime>

#include "core/game/entity/cards/chancecard.h"
#include "core/game/entity/cards/communitychestcard.h"

QVector<Card*> CardsManager::getDefaultCards(QWidget* parent)
{
	QVector<Card*> cards;

	CommunityChestCard *communityChestCard;
	ChanceCard *chanceCard;

	Action action;

	action.setMoveAction(0);
	communityChestCard = new CommunityChestCard(0, parent);
	communityChestCard->setText("Napreduj do Start polja\nPoberi 200€");
	communityChestCard->setAction(action);
	cards.append(communityChestCard);

	action.setCollectAction(200);
	communityChestCard = new CommunityChestCard(1, parent);
	communityChestCard->setText("Napaka banke v tvojo korist\nDobiš 200€");
	communityChestCard->setAction(action);
	cards.append(communityChestCard);

	action.setPayAction(50);
	communityChestCard = new CommunityChestCard(2, parent);
	communityChestCard->setText("Obisk pri zdravniku\nPlačaš 50€");
	communityChestCard->setAction(action);
	cards.append(communityChestCard);

	action.setCollectAction(50);
	communityChestCard = new CommunityChestCard(3, parent);
	communityChestCard->setText("Prodaja delnic\nDobiš 50€");
	communityChestCard->setAction(action);
	cards.append(communityChestCard);

	action.setGoToJailAction();
	communityChestCard = new CommunityChestCard(4, parent);
	communityChestCard->setText("Pojdi v zapor\nPojdni naravnost v zapor\nNe prečkaš Start polja");
	communityChestCard->setAction(action);
	cards.append(communityChestCard);

	action.setCollectAction(100);
	communityChestCard = new CommunityChestCard(5, parent);
	communityChestCard->setText("Praznični sklad\nDobiš 100€");
	communityChestCard->setAction(action);
	cards.append(communityChestCard);

	action.setCollectAction(20);
	communityChestCard = new CommunityChestCard(6, parent);
	communityChestCard->setText("Vračilo davka na dohodek\nDobiš 20€");
	communityChestCard->setAction(action);
	cards.append(communityChestCard);

	action.setCollectAction(10);
	communityChestCard = new CommunityChestCard(7, parent);
	communityChestCard->setText("Rojstni dan\nOd vsakega igralca pobereš 10€");
	communityChestCard->setAction(action);
	communityChestCard->setSpecialAction(SpecialAction::BIRTHDAY);
	cards.append(communityChestCard);

	action.setCollectAction(50);
	communityChestCard = new CommunityChestCard(8, parent);
	communityChestCard->setText("Opera\nOd vsakega igralca pobereš 50€");
	communityChestCard->setAction(action);
	communityChestCard->setSpecialAction(SpecialAction::BIRTHDAY);
	cards.append(communityChestCard);

	action.setCollectAction(100);
	communityChestCard = new CommunityChestCard(9, parent);
	communityChestCard->setText("Življensko zavarovanje\nDobiš 100€");
	communityChestCard->setAction(action);
	cards.append(communityChestCard);

	action.setPayAction(100);
	communityChestCard = new CommunityChestCard(10, parent);
	communityChestCard->setText("Stroški bolnišnice\nPlačaj 100€");
	communityChestCard->setAction(action);
	cards.append(communityChestCard);

	action.setPayAction(150);
	communityChestCard = new CommunityChestCard(11, parent);
	communityChestCard->setText("Stroški šolanja\nPlačaj 150€");
	communityChestCard->setAction(action);
	cards.append(communityChestCard);

	action.setCollectAction(25);
	communityChestCard = new CommunityChestCard(12, parent);
	communityChestCard->setText("Plačilo za svetovanje\nDobiš 25€");
	communityChestCard->setAction(action);
	cards.append(communityChestCard);

	action.setPayAction(0);
	communityChestCard = new CommunityChestCard(13, parent);
	communityChestCard->setText("Ulična popravila\nPlačaj 40€ za vsako hišo\nPlačaj 115€ za vsak hotel");
	communityChestCard->setAction(action);
	communityChestCard->setSpecialAction(SpecialAction::REPAIRS);
	cards.append(communityChestCard);

	action.setCollectAction(10);
	communityChestCard = new CommunityChestCard(14, parent);
	communityChestCard->setText("Drugo mesto na lepotnem tekmovanju\nDobiš 10€");
	communityChestCard->setAction(action);
	cards.append(communityChestCard);

	action.setCollectAction(100);
	communityChestCard = new CommunityChestCard(15, parent);
	communityChestCard->setText("Podeduješ 100€");
	communityChestCard->setAction(action);
	cards.append(communityChestCard);

	action.setMoveAction(0);
	chanceCard = new ChanceCard(16, parent);
	chanceCard->setText("Napreduj do Start polja\nPoberi 200€");
	chanceCard->setAction(action);
	cards.append(chanceCard);

	action.setMoveAction(24);
	chanceCard = new ChanceCard(17, parent);
	chanceCard->setText("Napreduj do Moravskih toplic\nČe prečkaš Start polje\nPobereš 200€");
	chanceCard->setAction(action);
	cards.append(chanceCard);

	action.setMoveAction(11);
	chanceCard = new ChanceCard(18, parent);
	chanceCard->setText("Napreduj do Bogenšperka\nČe prečkaš Start polje\nPobereš 200€");
	chanceCard->setAction(action);
	cards.append(chanceCard);

	action.setCollectAction(50);
	chanceCard = new ChanceCard(19, parent);
	chanceCard->setText("Banka ti plača 50€");
	chanceCard->setAction(action);
	cards.append(chanceCard);

	action.setMoveAction(-3);
	chanceCard = new ChanceCard(20, parent);
	chanceCard->setText("Premakni se 3 mesta nazaj");
	chanceCard->setAction(action);
	cards.append(chanceCard);

	action.setGoToJailAction();
	chanceCard = new ChanceCard(21, parent);
	chanceCard->setText("Pojdi v zapor\nPojdni naravnost v zapor\nNe prečkaš Start polja");
	chanceCard->setAction(action);
	cards.append(chanceCard);

	action.setPayAction(0);
	chanceCard = new ChanceCard(22, parent);
	chanceCard->setText("Splošna popravila\nPlačaj 25€ za vsako hišo\nPlačaj 100€ za vsak hotel");
	chanceCard->setAction(action);
	chanceCard->setSpecialAction(SpecialAction::REPAIRS);
	cards.append(chanceCard);

	action.setPayAction(15);
	chanceCard = new ChanceCard(23, parent);
	chanceCard->setText("Davek na revnost\nPlačaj 15€");
	chanceCard->setAction(action);
	cards.append(chanceCard);

	action.setMoveAction(5);
	chanceCard = new ChanceCard(24, parent);
	chanceCard->setText("Pojdi na\nŽelezniško postajo Jesenice\nČe prečkaš Start polje\nPobereš 200€");
	chanceCard->setAction(action);
	cards.append(chanceCard);

	action.setMoveAction(39);
	chanceCard = new ChanceCard(25, parent);
	chanceCard->setText("Pojdi v Portorož");
	chanceCard->setAction(action);
	cards.append(chanceCard);

	action.setPayAction(50);
	chanceCard = new ChanceCard(26, parent);
	chanceCard->setText("Izvoljen si za predsednika\nupravnega odbora\nVsakemu igralcu plačaj 50€");
	chanceCard->setAction(action);
	chanceCard->setSpecialAction(SpecialAction::CHAIRMAN);
	cards.append(chanceCard);

	action.setCollectAction(150);
	chanceCard = new ChanceCard(27, parent);
	chanceCard->setText("Sklad dozori\nDobiš 150€");
	chanceCard->setAction(action);
	cards.append(chanceCard);

	action.setCollectAction(100);
	chanceCard = new ChanceCard(28, parent);
	chanceCard->setText("Zmagaš na tekmovanju v\nreševanju križank\nDobiš 100€");
	chanceCard->setAction(action);
	cards.append(chanceCard);

	qsrand(QTime::currentTime().msec());

	int a, b;

	// Shuffle the deck
	for(int i = 0; i < cards.size(); i++)
	{
		a = qrand() % cards.size();
		b = qrand() % cards.size();

		Card *tmp = cards[a];

		cards[a] = cards[b];
		cards[b] = tmp;
	}

	return cards;
}
