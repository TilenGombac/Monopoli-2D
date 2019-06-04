#ifndef PLAYERPICKER_H
#define PLAYERPICKER_H

#include <QWidget>

class Player;

class PlayerPicker : public QWidget
{
	Q_OBJECT

	QVector<Player*> players;

public:
	PlayerPicker(const QVector<Player*> &setPlayers, const int &providerId,
					QWidget *parent = 0);

public slots:
	void handleButtonClick();

signals:
	void playerPicked(Player *player);

};

#endif // PLAYERPICKER_H
