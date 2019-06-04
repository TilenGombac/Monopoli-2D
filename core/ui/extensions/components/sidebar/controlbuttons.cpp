#include "controlbuttons.h"

#include <QWidget>

#include "core/utils/resourceutils.h"
#include "core/utils/windowUtils/windowutils.h"
#include "core/elements/buttonelement.h"
#include "core/settings/settingswindow.h"

ControlButtons::ControlButtons(QWidget *setParent)
{
	parent = setParent;

	settingsWindow = nullptr;

	addControlButtons();
}

ControlButtons::~ControlButtons()
{
	// Delete all 3 buttons
	for(int i = 0; i < 3; i++)
	{
		delete controlButtons[i];
	}

	delete settingsWindow;
	settingsWindow = nullptr;
}

void ControlButtons::addControlButtons()
{
	this->generateControlButton(EXIT);
	this->generateControlButton(MINIMIZE);
	this->generateControlButton(SETTINGS);

	// Button functionality
	parent->connect(controlButtons[EXIT], SIGNAL(clicked()),
					  parent, SLOT(quitApplication()));

	parent->connect(controlButtons[MINIMIZE], SIGNAL(clicked()),
					  parent, SLOT(setMinimized()));

	parent->connect(controlButtons[SETTINGS], SIGNAL(clicked()),
						this, SLOT(openSettings()));
}

void ControlButtons::generateControlButton(ControlButton controlButton)
{
	controlButtons[controlButton] = new ButtonElement(parent);

	int windowWidth = WindowUtils::getWindowSize().width();

	QSize btnSize = WindowUtils::BUTTON_SIZE;

	int btnWidth = btnSize.width();

	int paddingUnit = btnWidth * 0.5;

	int x = windowWidth - btnWidth * (controlButton + 1.5);
	int y = paddingUnit;

	QRect btnRect(QPoint(x, y), btnSize);

	controlButtons[controlButton]->setGeometry(btnRect);

	this->setControlButtonVisuals(controlButton);
}

void ControlButtons::setControlButtonVisuals(ControlButton controlButton)
{
	QString exitStylePath     = ":/css/css/buttons/exitButton.css";
	QString minimizeStylePath = ":/css/css/buttons/minimizeButton.css";
	QString settingsStylePath = ":/css/css/buttons/settingsButton.css";

	QString exitStyle     = ResourceUtils::getStyleSheet(exitStylePath);
	QString minimizeStyle = ResourceUtils::getStyleSheet(minimizeStylePath);
	QString settingsStyle = ResourceUtils::getStyleSheet(settingsStylePath);

	ButtonElement *currentButton = controlButtons[controlButton];

	switch(controlButton)
	{
		case EXIT:
			currentButton->setStyleSheet(exitStyle);
			break;

		case MINIMIZE:
			currentButton->setStyleSheet(minimizeStyle);
			break;

		case SETTINGS:
			currentButton->setStyleSheet(settingsStyle);
			break;
	}
}

void ControlButtons::openSettings()
{
	settingsWindow = new SettingsWindow(parent);
}
