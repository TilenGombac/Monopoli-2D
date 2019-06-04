#include "soundmanager.h"

#include <QCoreApplication>

#include "core/utils/errorUtils/errorhandler.h"

void SoundManager::play(const Sound &sound)
{
	QString path = "qrc:/sound/sound/";

	if(sound == Sound::AUTOMOBILE) path += "automobile.mp3";
	else if(sound == Sound::BATTLESHIP) path += "battleship.mp3";
	else if(sound == Sound::DEPOSIT) path+= "deposit.mp3";
	else if(sound == Sound::DICE) path += "dice.mp3";
	else if(sound == Sound::ERROR) path += "error.mp3";
	else if(sound == Sound::HELLO) path += "hello.mp3";
	else if(sound == Sound::HOME) path += "home.mp3";
	else if(sound == Sound::HOWITZER) path += "howitzer.mp3";
	else if(sound == Sound::INFORMATION) path += "information.mp3";
	else if(sound == Sound::SCOTTISH_TERRIER) path += "scottishTerrier.mp3";
	else if(sound == Sound::WARNING) path += "warning.mp3";
	else if(sound == Sound::WITHDRAW) path += "withdraw.mp3";
	else if(sound == Sound::CARD) path += "card.mp3";

	player.setMedia(QMediaContent(QUrl(path)));

	player.setVolume(volume);

	player.play();
}

void SoundManager::stop()
{
	player.stop();
}

void SoundManager::setVolume(const int &vol)
{
	volume = vol;

	player.setVolume(volume);
	ambientPlayer.setVolume(volume);
}

void SoundManager::startAmbient(const Sound &sound)
{
	QString path = "qrc:/sound/sound/";

	if(sound == Sound::AUTOMOBILE) path += "automobile.mp3";
	else if(sound == Sound::BATTLESHIP) path += "battleship.mp3";
	else if(sound == Sound::DEPOSIT) path+= "deposit.mp3";
	else if(sound == Sound::DICE) path += "dice.mp3";
	else if(sound == Sound::ERROR) path += "error.mp3";
	else if(sound == Sound::HELLO) path += "hello.mp3";
	else if(sound == Sound::HOME) path += "home.mp3";
	else if(sound == Sound::HOWITZER) path += "howitzer.mp3";
	else if(sound == Sound::INFORMATION) path += "information.mp3";
	else if(sound == Sound::SCOTTISH_TERRIER) path += "scottishTerrier.mp3";
	else if(sound == Sound::WARNING) path += "warning.mp3";
	else if(sound == Sound::WITHDRAW) path += "withdraw.mp3";
	else if(sound == Sound::CARD) path += "card.mp3";

	stopAmbient();

	ambientPlaylist.addMedia(QUrl(path));
	ambientPlaylist.setPlaybackMode(QMediaPlaylist::Loop);

	ambientPlayer.setPlaylist(&ambientPlaylist);

	ambientPlayer.setVolume(volume);

	ambientPlayer.play();
}

void SoundManager::stopAmbient()
{
	ambientPlaylist.clear();
	ambientPlayer.stop();
}

int SoundManager::getVolume()
{
	return volume;
}

QMediaPlayer SoundManager::player;
QMediaPlayer SoundManager::ambientPlayer;
QMediaPlaylist SoundManager::ambientPlaylist;
int SoundManager::volume = 50;
