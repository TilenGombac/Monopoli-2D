#ifndef VICTORYSCREEN_H
#define VICTORYSCREEN_H

#include <QWidget>

class VictoryScreen : public QWidget
{
	Q_OBJECT

	QString winner;
	QPixmap background;

public:
	explicit VictoryScreen(const QString &winner, const int &netWorth,
							QWidget *parent = 0);

protected:
	void paintEvent(QPaintEvent* event);

};

#endif // VICTORYSCREEN_H
