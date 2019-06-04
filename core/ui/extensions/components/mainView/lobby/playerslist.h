#ifndef PLAYERSLIST_H
#define PLAYERSLIST_H

#include <QWidget>

class Player;
class BoardManager;

class PlayersList : public QWidget
{
	BoardManager *boardManager;

public:
	PlayersList(BoardManager *setBoardManager, QWidget *parent = 0);

private:
	QRect playerRect(Player* player);

protected:
	void paintEvent(QPaintEvent *event);

};

#endif // PLAYERSLIST_H
