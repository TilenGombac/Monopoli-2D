#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <QMediaPlayer>
#include <QMediaPlaylist>

enum class Sound
{
	AUTOMOBILE,
	BATTLESHIP,
	DEPOSIT,
	DICE,
	ERROR,
	HELLO,
	HOME,
	HOWITZER,
	INFORMATION,
	SCOTTISH_TERRIER,
	WARNING,
	WITHDRAW,
	CARD
};

class SoundManager
{
	static QMediaPlayer player;
	static QMediaPlayer ambientPlayer;
	static QMediaPlaylist ambientPlaylist;

	static int volume;

public:
	static void play(const Sound &sound);
	static void stop();
	static void setVolume(const int &vol);

	static void startAmbient(const Sound &sound);
	static void stopAmbient();

	static int getVolume();
};

#endif // SOUNDMANAGER_H
