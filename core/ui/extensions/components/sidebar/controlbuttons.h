#ifndef CONTROLBUTTONS_H
#define CONTROLBUTTONS_H

#include <QObject>

class ButtonElement;
class SettingsWindow;

class ControlButtons : public QObject
{
	Q_OBJECT

	QWidget *parent;

	ButtonElement *controlButtons[3]; // 3 Control buttons

	SettingsWindow *settingsWindow;

public:
	// Enum instead of enum class for the "silent" conversion to int
	enum ControlButton
	{
		EXIT = 0,
		SETTINGS = 1,
		MINIMIZE = 2
	};

	ControlButtons(QWidget *setParent);
	~ControlButtons();

private:
	void addControlButtons();

	void generateControlButton(ControlButton controlButton);
	void setControlButtonVisuals(ControlButton controlButton);

public slots:
	void openSettings();

};

#endif // CONTROLBUTTONS_H
