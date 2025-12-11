#include "utils/SettingsUtils.h"

BikeInoSettings SettingsUtils::getBikeInoSettings()
{
    struct BikeInoSettings bikeInoSettings;

    Preferences prefs;
    prefs.begin("BikeIno", true);	// read-only
    bikeInoSettings.soundSetting = prefs.getBool("sound", false);
    bikeInoSettings.imperialUnitsSetting = prefs.getBool("imperial", false);
    bikeInoSettings.wheelTyreIndex = prefs.getUChar("wheelTyre", 4); // Default: 700 x 25C
    prefs.end();

    return bikeInoSettings;
}

void SettingsUtils::storeSoundSetting(bool value)
{
    //Save the modified sound setting
    Preferences prefs;
    prefs.begin("BikeIno");
    prefs.putBool("sound", value);
    prefs.end();
}

void SettingsUtils::storeImperialUnitsSetting(bool value)
{
    //Save the modified imperial units setting
    Preferences prefs;
    prefs.begin("BikeIno");
    prefs.putBool("imperial", value);
    prefs.end();
}

void SettingsUtils::storeWheelTyreSetting(uint8_t index)
{
    //Save the modified wheel tyre setting
    Preferences prefs;
    prefs.begin("BikeIno");
    prefs.putUChar("wheelTyre", index);
    prefs.end();
}

BikeInoSettings SettingsUtils::resetSettings()
{
    struct BikeInoSettings bikeInoSettings;
    bikeInoSettings.soundSetting = false;
    bikeInoSettings.imperialUnitsSetting = false;
    bikeInoSettings.wheelTyreIndex = 4; // Default: 700 x 25C

    Preferences prefs;
    prefs.begin("BikeIno");
    prefs.putBool("sound", bikeInoSettings.soundSetting);
    prefs.putBool("imperial", bikeInoSettings.imperialUnitsSetting);
    prefs.putUChar("wheelTyre", bikeInoSettings.wheelTyreIndex);
    prefs.end();

    return bikeInoSettings;
}