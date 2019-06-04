#ifndef LOBBYUTILS_H
#define LOBBYUTILS_H

class Token;
class Field;
class QWidget;
class QComboBox;
class QPushButton;

class LobbyUtils
{
public:
	static void initializeTokenPicker(QComboBox *tokenPicker);
	static void setFont(QWidget *item);
	static void initializeQuitButton(QPushButton *quitButton);
	static void initializeStartButton(QPushButton *startButton);
	static Token* getSelectedToken(QComboBox *tokenPicker, Field *start,
									QWidget *parent);

};

#endif // LOBBYUTILS_H
