#include "Arduino.h"
#include "M5ez.h"
#include "ezTime.h"

#include "screens/BikeInoScreen.h"
#include "utils/SettingsUtils.h"

class SettingsPicker : public BikeInoScreen {
public:    
    static void runOnce(String pickerName);        
private:
    static String _displayPickerMenu(String pickerName);

    static SettingsUtils _settingsUtils;

    /////////////
    // Pickers //
    /////////////

    // General picker settings
    static String _pickerButtons;
    static GFXfont _pickerFont;
    static int8_t _pickerCheckType;
    static String _pickerCheckButtonName;

    static void _displayWifiPicker();
    static bool _advancedDisplayWifiPicker(ezMenu* callingMenu);    
    static void _displayClockPicker();
    static bool _advancedDisplayClockPicker(ezMenu* callingMenu);
    static void _displayBatteryPicker();
    static bool _advancedDisplayBatteryPicker(ezMenu* callingMenu);    
    static void _displayBacklightPicker();
    static bool _advancedDisplayBacklightPicker(ezMenu* callingMenu);
    static void _displayThemePicker();
    static bool _advancedDisplayThemePicker(ezMenu* callingMenu);
    static void _displayFactoryDefaultsPicker();
    static bool _advancedDisplayFactoryDefaultsPicker(ezMenu* callingMenu);
    static void _displaySoundPicker();
    static bool _advancedDisplaySoundPicker(ezMenu* callingMenu);
    static void _displayImperialUnitsPicker();
    static bool _advancedDisplayImperialUnitsPicker(ezMenu* callingMenu);
};