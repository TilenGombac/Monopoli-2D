#include "victoryscreen.h"

#include <QFile>
#include <QFont>
#include <QDate>
#include <QPainter>
#include <QTextStream>
#include <QApplication>

#include "core/utils/resourceutils.h"
#include "core/elements/buttonelement.h"
#include "core/utils/windowUtils/windowutils.h"

VictoryScreen::VictoryScreen(const QString &winner, const int &netWorth,
								QWidget *parent)
	: QWidget(parent)
{
	this->winner = winner;

	background.load(":/img/img/screens/gameOver.jpg");

	this->setFixedSize(background.width(), background.height());

	this->setWindowFlags(Qt::FramelessWindowHint);

	QString exitStylePath = ":/css/css/buttons/exitButton.css";

	QString exitStyle = ResourceUtils::getStyleSheet(exitStylePath);

	ButtonElement *exit = new ButtonElement(this);

	exit->setFixedSize(WindowUtils::BUTTON_SIZE);

	QRect exitRect(QPoint(this->width() - 30, 5), WindowUtils::BUTTON_SIZE);

	exit->setGeometry(exitRect);

	exit->setStyleSheet(exitStyle);

	connect(exit, SIGNAL(clicked()),
				QApplication::instance(), SLOT(quit()));

	WindowUtils::centerToScreen(this);

	QFile file("highscore.txt");

	if(file.open(QIODevice::ReadWrite))
	{
		QTextStream inStream(&file);

		QString existing = inStream.readAll();

		QString existingHighscore = existing.split("").last();
		bool ok;
		float highscore = existingHighscore.toFloat(&ok);

		if(!ok || highscore < netWorth)
		{
			QDate date;
			QString dateString = date.currentDate().toString("d'. 'M'. 'yyyy");

			QString data = "[" + winner + ", " + dateString + "] ";
			data += "Net vrednost: " + QString::number(netWorth);

			QTextStream outStream(&file);
			outStream << data;

		}

		file.close();
	}
}

void VictoryScreen::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);

	QPainter painter(this);

	painter.drawPixmap(0, 0, background);

	int x = 20;
	int y = this->height() - 60;

	QFont font = painter.font();
	font.setPointSize(24);
	font.setBold(true);

	painter.setFont(font);
	painter.setPen(Qt::white);

	painter.drawText(x, y, this->width() - 20, 60, Qt::AlignVCenter,
						"Zmagovalec je " + this->winner);
}
