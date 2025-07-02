#ifndef SettingsUtils_h
#define SettingsUtils_h

#include "Arduino.h"
#include "Preferences.h"

#include "BikeInoBase.h"

class SettingsUtils : public BikeInoBase {
    
    public:
        BikeInoSettings getBikeInoSettings();
        BikeInoSettings resetSettings();
        
        void storeSoundSetting(bool value);
        void storeImperialUnitsSetting(bool value);

    private:  
        
};

#endif