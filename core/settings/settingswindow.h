#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QWidget>

class QLabel;
class QSlider;
class QComboBox;
class QPushButton;

class SettingsWindow : public QWidget
{
	Q_OBJECT

	QSlider* volumeSlider;

	QLabel* volumeLabel;

	QLabel* rulesLinkLabel;

	QComboBox* windowSizeSelector;

	QPushButton *done;


public:
	SettingsWindow(QWidget *parent = 0);

private:
	void fillWindowSizeSelector();

protected:
	void paintEvent(QPaintEvent *event);

public slots:
	void changeVolume(int volume);
	void saveWindowSizePreference();

};

#endif // SETTINGSWINDOW_H
