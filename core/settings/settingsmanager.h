#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

enum class Preference
{
	VOLUME,
	FIELD_WIDTH,
	FIELD_HEIGHT
};

class SettingsManager
{
public:
	static int getPreference(const Preference &preference);
	static void writePreference(const Preference &preference,
									const int &value);

private:
	static bool preferencesExist();

};

#endif // SETTINGSMANAGER_H
