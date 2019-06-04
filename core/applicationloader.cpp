#include "applicationloader.h"

#include "core/ui/mainwindow.h"
#include "utils/constantutils.h"
#include "core/settings/settingsmanager.h"
#include "core/utils/windowUtils/windowutils.h"

#include "core/sounds/soundmanager.h"

void ApplicationLoader::load(MainWindow *window)
{
	int volume = SettingsManager::getPreference(Preference::VOLUME);

	SoundManager::setVolume(volume);

	window->show();
}
