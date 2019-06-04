#include "settingswindow.h"

#include <QLabel>
#include <QSlider>
#include <QPainter>
#include <QComboBox>
#include <QPushButton>

#include "core/utils/constantutils.h"
#include "core/settings/settingsmanager.h"
#include "core/utils/windowUtils/windowutils.h"
#include "core/notifications/notificationmanager.h"
#include "core/utils/settingsUtils/settingswindowutils.h"

#include "core/sounds/soundmanager.h"

SettingsWindow::SettingsWindow(QWidget *parent)
	: QWidget(parent)
{
	this->raise();

	this->setGeometry(SettingsWindowUtils::getWindowGeometry());

	done = new QPushButton("Zapri", this);

	int x = (this->geometry().width() - 100) / 2;
	int y = (this->geometry().height() - 40) / 2 + ConstantUtils::FIELD_HEIGHT;

	done->setGeometry(x, y, 100, 40);

	x = (this->geometry().width() - 300) / 2;

	y -= 100;

	windowSizeSelector = new QComboBox(this);
	windowSizeSelector->setGeometry(x, y, 300, 40);
	windowSizeSelector->setFont(QFont("Verdana", 14));
	fillWindowSizeSelector();

	connect(windowSizeSelector, SIGNAL(currentIndexChanged(int)),
				this, SLOT(saveWindowSizePreference()));

	y -= 50;

	volumeSlider = new QSlider(Qt::Horizontal, this);
	volumeSlider->setGeometry(x, y, 300, 10);
	volumeSlider->setMinimum(0);
	volumeSlider->setMaximum(100);
	volumeSlider->setValue(SoundManager::getVolume());

	connect(volumeSlider, SIGNAL(valueChanged(int)),
				this, SLOT(changeVolume(int)));

	y -= 40;

	volumeLabel = new QLabel("Nastavitev glasnosti", this);
	volumeLabel->setGeometry(x, y, 300, 40);
	volumeLabel->setAlignment(Qt::AlignCenter);
	volumeLabel->setFont(QFont("Verdana", 14));
	volumeLabel->setStyleSheet("QLabel { color: #FFFFFF; }");

	y -= 50;

	rulesLinkLabel = new QLabel("<a href=\"https://en.wikibooks.org/wiki/Monopoly/Official_Rules\">"
								"<span style=\"color: #56E39F; text-decoration: none; \">Povezava do pravil</span></a>", this);
	rulesLinkLabel->setTextFormat(Qt::RichText);
	rulesLinkLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
	rulesLinkLabel->setOpenExternalLinks(true);
	rulesLinkLabel->setGeometry(x, y, 300, 40);
	rulesLinkLabel->setAlignment(Qt::AlignCenter);
	rulesLinkLabel->setFont(QFont("Verdana", 14));

	connect(done, SIGNAL(clicked()), this, SLOT(close()));

	this->show();
}

void SettingsWindow::fillWindowSizeSelector()
{
	int fieldWidth  = ConstantUtils::FIELD_WIDTH;
	int fieldHeight = ConstantUtils::FIELD_HEIGHT;

	windowSizeSelector->addItem("Velikost aplikacije",
									QSize(fieldWidth, fieldHeight));

	int sidebarWidth = ConstantUtils::SIDEBAR_WIDTH;
	int windowWidth, windowHeight;

	QString sizeText;

	for(int i = 84; i >= 60; i -= 2)
	{
		fieldWidth  = i;
		fieldHeight = 3 * fieldWidth / 2;

		windowWidth  = 12 * fieldWidth + sidebarWidth;
		windowHeight = 12 * fieldWidth;

		sizeText = QString::number(windowWidth) + " x "
					+ QString::number(windowHeight);

		windowSizeSelector->addItem(sizeText,
										QSize(fieldWidth, fieldHeight));
	}
}

void SettingsWindow::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);

	QPainter painter(this);

	SettingsWindowUtils::drawBackground(&painter);

	painter.setPen(Qt::white);
	painter.setFont(QFont("Arial Black", 48));
	painter.drawText(0, 0, this->width(), 200, Qt::AlignCenter,
						"NASTAVITVE");
}

void SettingsWindow::changeVolume(int volume)
{
	SoundManager::setVolume(volume);

	SettingsManager::writePreference(Preference::VOLUME, volume);
}

void SettingsWindow::saveWindowSizePreference()
{
	NotificationManager::pushNotification("Sprememba velikosti",
											"Ta sprememba bo upoštevana ob "
											"naslednjem zagonu aplikacije.",
												NotificationType::INFORMATION);

	QSize fieldSize = windowSizeSelector->currentData().toSize();

	SettingsManager::writePreference(Preference::FIELD_WIDTH,
										fieldSize.width());
	SettingsManager::writePreference(Preference::FIELD_HEIGHT,
										fieldSize.height());
}
