#include "tradeview.h"

#include <QLabel>
#include <QTimer>
#include <QPainter>
#include <QToolButton>

#include "core/utils/windowUtils/windowutils.h"
#include "core/game/entity/fields/possessablefield.h"

TradeView::TradeView(QString provider, QVector<Field*> fields, TradePacket packet,
						QWidget* parent)
	: QWidget(parent)
{
	this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::Window);

	this->setFixedSize(450, 350);

	WindowUtils::centerToScreen(this);

	QFont font("Verdana", 10);

	tradePacket = packet;

	OfferPair provided = packet.getProvided(fields);
	OfferPair returned = packet.getReturned(fields);

	QString amount1 = QString::number(provided.getAmount());
	QString amount2 = QString::number(returned.getAmount());

	int w = this->width() / 2 - 20;

	titleLabel = new QLabel("Ogled ponudbe", this);
	titleLabel->setGeometry(0, 0, this->width(), 50);
	titleLabel->setAlignment(Qt::AlignCenter);
	titleLabel->setFont(QFont("Verdana", 18));

	providerLabel = new QLabel("Predlagal " + provider, this);
	providerLabel->setGeometry(20, 60, w, 30);
	providerLabel->setAlignment(Qt::AlignVCenter);
	providerLabel->setFont(font);

	inReturnLabel = new QLabel("V zameno za", this);
	inReturnLabel->setGeometry(w + 20, 60, w, 30);
	inReturnLabel->setAlignment(Qt::AlignCenter);
	inReturnLabel->setFont(font);

	providedAmount = new QLabel("Ponujena vsota: " + amount1 + "€", this);
	providedAmount->setGeometry(20, 110, w, 30);
	providedAmount->setAlignment(Qt::AlignVCenter);
	providedAmount->setFont(font);

	returnedAmount = new QLabel("Predlagana vsota: " + amount2 + "€", this);
	returnedAmount->setGeometry(w + 20, 110, w, 30);
	returnedAmount->setAlignment(Qt::AlignVCenter);
	returnedAmount->setFont(font);

	providedFields = new QLabel(this);
	providedFields->setGeometry(20, 160, w, 150);
	providedFields->setAlignment(Qt::AlignTop);
	providedFields->setFont(font);

	QString tmpText = "Ponujena polja\n";

	for(PossessableField* field : provided.getFields())
	{
		tmpText += " - " + field->getName() + "\n";
	}

	providedFields->setText(tmpText);


	returnedFields = new QLabel(this);
	returnedFields->setGeometry(w + 20, 160, w, 150);
	returnedFields->setAlignment(Qt::AlignTop);
	returnedFields->setFont(font);

	tmpText = "Ponujena polja\n";

	for(PossessableField* field : returned.getFields())
	{
		tmpText += " - " + field->getName() + "\n";
	}

	returnedFields->setText(tmpText);


	denyButton = new QToolButton(this);
	denyButton->setText("Zavrni");
	denyButton->setGeometry(0, 320, this->width() / 2, 30);
	denyButton->setFont(font);

	acceptButton = new QToolButton(this);
	acceptButton->setText("Sprejmi");
	acceptButton->setGeometry(this->width() / 2, 320, this->width() / 2, 30);
	acceptButton->setFont(font);


	autoCancelProgress = new QProgressBar(this);
	autoCancelProgress->setGeometry(0, 0, this->width(), 4);

	autoCancelTimer     = new QTimer(this);
	updateProgressTimer = new QTimer(this);

	autoCancelTimer->start(25000);
	updateProgressTimer->start(10);

	time.start();

	connect(updateProgressTimer, SIGNAL(timeout()),
				this, SLOT(updateProgress()));


	connect(acceptButton, SIGNAL(clicked()),
				this, SLOT(accept()));

	connect(denyButton, SIGNAL(clicked()),
				this, SLOT(deny()));

	connect(autoCancelTimer, SIGNAL(timeout()),
				this, SLOT(deny()));

	this->show();
}

void TradeView::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event)
}

void TradeView::accept()
{
	emit accepted(tradePacket);

	this->deleteLater();
}

void TradeView::deny()
{
	emit denied(tradePacket);

	this->deleteLater();
}

void TradeView::updateProgress()
{
	autoCancelProgress->setValue(time.elapsed() / 150);
}
