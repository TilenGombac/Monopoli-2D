#ifndef BOARDDISPLAY_H
#define BOARDDISPLAY_H

#include <QWidget>

class BoardManager;

class BoardDisplay : public QWidget
{
	BoardManager *boardManager;

public:
	BoardDisplay(BoardManager *manager, QWidget *parent = 0);

	BoardManager *getBoardManager();

private:
	void showBoard();

protected:
	void paintEvent(QPaintEvent *event);

};

#endif // BOARDDISPLAY_H
