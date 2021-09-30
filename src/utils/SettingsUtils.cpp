#include "utils/SettingsUtils.h"

BikeInoSettings SettingsUtils::getBikeInoSettings()
{
    struct BikeInoSettings bikeInoSettings;

    Preferences prefs;
    prefs.begin("BikeIno", true);	// read-only
    bikeInoSettings.soundSetting = prefs.getBool("sound", false);
    bikeInoSettings.imperialUnitsSetting = prefs.getBool("imperial", false);
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

BikeInoSettings SettingsUtils::resetSettings()
{
    struct BikeInoSettings bikeInoSettings;
    bikeInoSettings.soundSetting = false;
    bikeInoSettings.imperialUnitsSetting = false;

    Preferences prefs;
    prefs.begin("BikeIno");
    prefs.putBool("sound", bikeInoSettings.soundSetting);
    prefs.putBool("imperial", bikeInoSettings.imperialUnitsSetting);
    prefs.end();

    return bikeInoSettings;
}