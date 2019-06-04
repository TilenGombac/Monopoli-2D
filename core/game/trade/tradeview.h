#ifndef TRADEVIEW_H
#define TRADEVIEW_H

#include <QTime>
#include <QWidget>
#include <QProgressBar>

#include "network/packets/tradepacket.h"

class QLabel;
class QTimer;
class QToolButton;

class TradeView : public QWidget
{
	Q_OBJECT

	QTimer* autoCancelTimer;
	QTimer* updateProgressTimer;

	QProgressBar *autoCancelProgress;

	QTime time;

	QLabel* titleLabel;
	QLabel* providerLabel;
	QLabel* inReturnLabel;
	QLabel* providedAmount;
	QLabel* returnedAmount;
	QLabel* providedFields;
	QLabel* returnedFields;

	QToolButton* acceptButton;
	QToolButton* denyButton;

	TradePacket tradePacket;

public:
	TradeView(QString provider, QVector<Field*> fields,
				TradePacket packet, QWidget* parent = 0);

protected:
	void paintEvent(QPaintEvent *event);

private slots:
	void accept();
	void deny();
	void updateProgress();

signals:
	void accepted(TradePacket packet);
	void denied(TradePacket packet);

};

#endif // TRADEVIEW_H
