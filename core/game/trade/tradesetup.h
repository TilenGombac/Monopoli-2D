#ifndef TRADESETUP_H
#define TRADESETUP_H

#include <QWidget>

#include "core/game/trade/offerpair.h"

class Offer;
class Field;
class Player;
class QLabel;
class QSpinBox;
class QToolButton;
class BoardManager;
class PlayerPicker;

enum class CurrentlyPicking
{
	NONE      = 0,
	PROVIDER  = 1,
	RECIPIENT = 2
};

class TradeSetup : public QWidget
{
	Q_OBJECT

	PlayerPicker *playerPicker;

	QSpinBox *providerSpinBox;
	QSpinBox *recipientSpinBox;

	QLabel *titleProvider;
	QLabel *titleRecipient;

	QLabel *currencyProvider;
	QLabel *currencyRecipient;

	QLabel *fieldsProvider;
	QLabel *fieldsRecipient;

	QToolButton *saveProvider;
	QToolButton *saveRecipient;
	QToolButton *sendOfferButton;
	QToolButton *cancelOfferButton;

	CurrentlyPicking picking;

	Offer *offer;

public:
	TradeSetup(BoardManager *bm, Player *provider, QWidget *parent = 0);
	~TradeSetup();

protected:
	void paintEvent(QPaintEvent *event);

public slots:
	void addField(Field *selectedField);
	void sendOffer();
	void changeAmount(int value);
	void setRecipient(Player* player);
	void saveCurrentPicks();
	void cancelOffer();

signals:
	void offerSent();
	void offerSent(Offer* o);

	void offerCancelled();

};

#endif // TRADESETUP_H
