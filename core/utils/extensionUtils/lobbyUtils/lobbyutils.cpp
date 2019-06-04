#include "lobbyutils.h"

#include <QPixmap>
#include <QComboBox>
#include <QPushButton>

#include "core/game/entity/tokens/automobiletoken.h"
#include "core/game/entity/tokens/battleshiptoken.h"
#include "core/game/entity/tokens/howitzertoken.h"
#include "core/game/entity/tokens/scottishterriertoken.h"
#include "core/game/entity/tokens/thimbletoken.h"
#include "core/game/entity/tokens/tophattoken.h"
#include "core/utils/tokenUtils/tokenpixmaputils.h"

void LobbyUtils::initializeTokenPicker(QComboBox *tokenPicker)
{
		QPixmap pixmap;

		pixmap = TokenPixmapUtils::getAutomobilePixmap();
		tokenPicker->addItem(QIcon(pixmap), "Avtomobil");

		pixmap = TokenPixmapUtils::getBattleshipPixmap();
		tokenPicker->addItem(QIcon(pixmap), "Bojna ladja");

		pixmap = TokenPixmapUtils::getHowitzerPixmap();
		tokenPicker->addItem(QIcon(pixmap), "Top");

		pixmap = TokenPixmapUtils::getScottishTerrierPixmap();
		tokenPicker->addItem(QIcon(pixmap), "Škotski terier");

		pixmap = TokenPixmapUtils::getThimblePixmap();
		tokenPicker->addItem(QIcon(pixmap), "Naprstnik");

		pixmap = TokenPixmapUtils::getTopHatPixmap();
		tokenPicker->addItem(QIcon(pixmap), "Klobuk");

		tokenPicker->setFont(QFont("Verdana", 18));
}

void LobbyUtils::setFont(QWidget *item)
{
	item->setFont(QFont("Verdana", 18));
}

void LobbyUtils::initializeQuitButton(QPushButton *quitButton)
{
	int quitWidth  = 150;
	int startWidth = 250;

	int parentWidth = quitButton->parentWidget()->width();

	int x = (parentWidth - quitWidth - startWidth) / 2;

	quitButton->setGeometry(x, 600, quitWidth, 40);
	quitButton->setShortcut(QKeySequence(Qt::Key_Escape));

	setFont(quitButton);
}

void LobbyUtils::initializeStartButton(QPushButton *startButton)
{
	int quitWidth  = 150;
	int startWidth = 250;

	int parentWidth = startButton->parentWidget()->width();

	int x = (parentWidth - startWidth) / 2 + (quitWidth / 2) + 30;

	startButton->setGeometry(x, 600, startWidth, 40);
	startButton->setShortcut(QKeySequence(Qt::Key_Escape));

	startButton->setFont(QFont("Verdana", 18, QFont::Bold));
}

Token* LobbyUtils::getSelectedToken(QComboBox *tokenPicker, Field *start,
										QWidget *parent)
{
	QString currentText = tokenPicker->currentText();

	if(currentText == "Avtomobil")
		return new AutomobileToken(start, parent);

	if(currentText == "Bojna ladja")
		return new BattleshipToken(start, parent);

	if(currentText == "Top")
		return new HowitzerToken(start, parent);

	if(currentText == "Škotski terier")
		return new ScottishTerrierToken(start, parent);

	if(currentText == "Naprstnik")
		return new ThimbleToken(start, parent);

	// Only other option
	return new TopHatToken(start, parent);
}
