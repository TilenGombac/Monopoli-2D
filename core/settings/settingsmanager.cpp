#include "settingsmanager.h"

#include <QFile>
#include <QDataStream>

int SettingsManager::getPreference(const Preference &preference)
{
	QFile preferencesFile("preferences.bin");

	int volume      = 50;
	int fieldWidth  = 80;
	int fieldHeight = 120;

	if(preferencesExist())
	{
		preferencesFile.open(QIODevice::ReadOnly);

		QDataStream stream(&preferencesFile);

		stream >> volume >> fieldWidth >> fieldHeight;

		preferencesFile.close();
	}
	else
	{
		writePreference(Preference::VOLUME, volume);
		writePreference(Preference::FIELD_WIDTH, fieldWidth);
		writePreference(Preference::FIELD_HEIGHT, fieldHeight);
	}

	if(preference == Preference::VOLUME)
		return volume;

	else if(preference == Preference::FIELD_WIDTH)
		return fieldWidth;

	else if(preference == Preference::FIELD_HEIGHT)
		return fieldHeight;

	else
		return 0;
}

void SettingsManager::writePreference(const Preference &preference,
										const int &value)
{
	QFile preferencesFile("preferences.bin");

	int volume      = 50;
	int fieldWidth  = 80;
	int fieldHeight = 120;

	if(preferencesExist())
	{
		volume      = getPreference(Preference::VOLUME);
		fieldWidth  = getPreference(Preference::FIELD_WIDTH);
		fieldHeight = getPreference(Preference::FIELD_HEIGHT);
	}

	if(preference == Preference::VOLUME)
		volume = value;

	else if(preference == Preference::FIELD_WIDTH)
		fieldWidth = value;

	else if(preference == Preference::FIELD_HEIGHT)
		fieldHeight = value;

	preferencesFile.open(QIODevice::WriteOnly | QIODevice::Truncate);

	QDataStream stream(&preferencesFile);

	stream << volume;
	stream << fieldWidth;
	stream << fieldHeight;

	preferencesFile.close();
}

bool SettingsManager::preferencesExist()
{
	QFile preferencesFile("preferences.bin");

	return preferencesFile.exists();
}
