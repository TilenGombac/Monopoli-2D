#ifndef PARTICIPANTSVIEW_H
#define PARTICIPANTSVIEW_H

#include <QWidget>

class Player;

class ParticipantsView : public QWidget
{
	Q_OBJECT

	// Player + ping
	QVector<QPair<Player*, int>> participants;
	bool multiplayer;

	int playerTurn;

public:
	ParticipantsView(const bool &setMultiplayer, QWidget *parent = 0);
	ParticipantsView(const QVector<Player*> &players, QWidget *parent = 0);

	QVector<Player*> getParticipants();

protected:
	void paintEvent(QPaintEvent *event);

public slots:
	void addParticipant(Player *player);
	void removeParticipant(Player *player);
	void setPlayerTurn(Player* player);
	void refreshPing(int playerId, int ping);
	void refresh();

};

#endif // PARTICIPANTSVIEW_H
