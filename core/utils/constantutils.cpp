#include "constantutils.h"

#include "core/settings/settingsmanager.h"

int ConstantUtils::FIELD_WIDTH  = SettingsManager::getPreference(Preference::FIELD_WIDTH);
int ConstantUtils::FIELD_HEIGHT = SettingsManager::getPreference(Preference::FIELD_HEIGHT);

int ConstantUtils::SIDEBAR_WIDTH = 360;
