#ifndef SettingsScreen_h
#define SettingsScreen_h

#include "Arduino.h"

#include "screens/BikeInoScreen.h"
#include "utils/SettingsUtils.h"

class SettingsScreen : public BikeInoScreen {

public:
    SettingsScreen(SettingsUtils* settingsUtils);

    // Override base class methods
    void createUI() override;
    void destroyUI() override;
    void update() override;

    // Set settings
    void setSettings(BikeInoSettings settings);

private:
    SettingsUtils* _settingsUtils;

    // UI widgets
    lv_obj_t* _titleLabel;
    lv_obj_t* _settingsContainer;

    // Sound setting row
    lv_obj_t* _soundLabel;
    lv_obj_t* _soundSwitch;

    // Units setting row
    lv_obj_t* _unitsLabel;
    lv_obj_t* _unitsSwitch;

    // Wheel tyre dropdown
    lv_obj_t* _wheelLabel;
    lv_obj_t* _wheelDropdown;

    // Action buttons
    lv_obj_t* _deleteAllButton;
    lv_obj_t* _factoryResetButton;

    // Message box for confirmation
    lv_obj_t* _msgbox;

    // Track which action is pending confirmation
    int _pendingAction; // 0 = none, 1 = delete all, 2 = factory reset

    void _createSettingsRows();
    void _createActionButtons();
    void _showDeleteAllConfirmation();
    void _showFactoryResetConfirmation();
    void _deleteAllTrips();
    void _performFactoryReset();

    // Event handlers
    static void _soundSwitchHandler(lv_event_t* e);
    static void _unitsSwitchHandler(lv_event_t* e);
    static void _wheelDropdownHandler(lv_event_t* e);
    static void _deleteAllButtonHandler(lv_event_t* e);
    static void _factoryResetButtonHandler(lv_event_t* e);
    static void _msgboxHandler(lv_event_t* e);
};

#endif
