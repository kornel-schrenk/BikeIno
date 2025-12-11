#include "screens/SettingsScreen.h"
#include <SD.h>

// Wheel tyre options from UX spec
static const char* WHEEL_TYRE_OPTIONS =
    "26 x 1.75\n"
    "26 x 2.0\n"
    "26 x 2.125\n"
    "27.5 x 2.25\n"
    "700 x 25C\n"
    "700 x 28C\n"
    "700 x 32C\n"
    "700 x 35C\n"
    "700 x 38C\n"
    "700 x 47C\n"
    "29 x 2.35";

SettingsScreen::SettingsScreen(SettingsUtils* settingsUtils) {
    _settingsUtils = settingsUtils;

    _titleLabel = nullptr;
    _settingsContainer = nullptr;
    _soundLabel = nullptr;
    _soundSwitch = nullptr;
    _unitsLabel = nullptr;
    _unitsSwitch = nullptr;
    _wheelLabel = nullptr;
    _wheelDropdown = nullptr;
    _deleteAllButton = nullptr;
    _factoryResetButton = nullptr;
    _msgbox = nullptr;
    _pendingAction = 0;
}

void SettingsScreen::setSettings(BikeInoSettings settings) {
    _settings = settings;
}

void SettingsScreen::createUI() {
    createBaseLayout();
    _createSettingsRows();
    _createActionButtons();
}

void SettingsScreen::destroyUI() {
    BikeInoScreen::destroyUI();

    _titleLabel = nullptr;
    _settingsContainer = nullptr;
    _soundLabel = nullptr;
    _soundSwitch = nullptr;
    _unitsLabel = nullptr;
    _unitsSwitch = nullptr;
    _wheelLabel = nullptr;
    _wheelDropdown = nullptr;
    _deleteAllButton = nullptr;
    _factoryResetButton = nullptr;
    _msgbox = nullptr;
    _pendingAction = 0;
}

void SettingsScreen::update() {
    // Settings are static - no periodic update needed
}

void SettingsScreen::_createSettingsRows() {
    // Make content pane scrollable for settings
    lv_obj_add_flag(_contentPane, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_scroll_dir(_contentPane, LV_DIR_VER);

    // Title
    _titleLabel = lv_label_create(_contentPane);
    lv_label_set_text(_titleLabel, "Settings");
    lv_obj_set_style_text_font(_titleLabel, &lv_font_montserrat_18, 0);
    lv_obj_set_style_text_color(_titleLabel, COLOR_TEXT_PRIMARY, 0);
    lv_obj_align(_titleLabel, LV_ALIGN_TOP_MID, 0, 0);

    // Settings container - compact height
    _settingsContainer = lv_obj_create(_contentPane);
    lv_obj_set_size(_settingsContainer, CONTENT_PANE_WIDTH - 16, 115);
    lv_obj_set_pos(_settingsContainer, 0, 26);
    lv_obj_set_style_bg_color(_settingsContainer, lv_color_hex(0x2A2A2A), 0);
    lv_obj_set_style_border_width(_settingsContainer, 0, 0);
    lv_obj_set_style_radius(_settingsContainer, 8, 0);
    lv_obj_set_style_pad_all(_settingsContainer, 8, 0);
    lv_obj_clear_flag(_settingsContainer, LV_OBJ_FLAG_SCROLLABLE);

    // Wheel tyre row at top (y=0)
    _wheelLabel = lv_label_create(_settingsContainer);
    lv_label_set_text(_wheelLabel, "Wheel Tyre");
    lv_obj_set_style_text_font(_wheelLabel, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(_wheelLabel, COLOR_TEXT_PRIMARY, 0);
    lv_obj_set_pos(_wheelLabel, 0, 4);

    _wheelDropdown = lv_dropdown_create(_settingsContainer);
    lv_dropdown_set_options(_wheelDropdown, WHEEL_TYRE_OPTIONS);
    lv_obj_set_size(_wheelDropdown, 120, 28);
    lv_obj_align(_wheelDropdown, LV_ALIGN_TOP_RIGHT, 0, 0);
    lv_obj_set_style_text_font(_wheelDropdown, &lv_font_montserrat_12, 0);
    lv_obj_set_style_bg_color(_wheelDropdown, lv_color_hex(0x404040), 0);
    lv_obj_set_style_text_color(_wheelDropdown, COLOR_TEXT_PRIMARY, 0);
    lv_obj_set_style_border_color(_wheelDropdown, COLOR_INACTIVE, 0);

    // Set dropdown list style
    lv_obj_t* list = lv_dropdown_get_list(_wheelDropdown);
    if (list != nullptr) {
        lv_obj_set_style_bg_color(list, lv_color_hex(0x404040), 0);
        lv_obj_set_style_text_color(list, COLOR_TEXT_PRIMARY, 0);
        lv_obj_set_style_text_font(list, &lv_font_montserrat_12, 0);
    }

    lv_dropdown_set_selected(_wheelDropdown, _settings.wheelTyreIndex);
    lv_obj_add_event_cb(_wheelDropdown, _wheelDropdownHandler, LV_EVENT_VALUE_CHANGED, this);

    // Sound and Units in 2-column layout below wheel tyre
    // Row 1: Labels (y=38)
    _soundLabel = lv_label_create(_settingsContainer);
    lv_label_set_text(_soundLabel, "Sound");
    lv_obj_set_style_text_font(_soundLabel, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(_soundLabel, COLOR_TEXT_PRIMARY, 0);
    lv_obj_set_pos(_soundLabel, 0, 42);

    _unitsLabel = lv_label_create(_settingsContainer);
    lv_label_set_text(_unitsLabel, _settings.imperialUnitsSetting ? "mph" : "km/h");
    lv_obj_set_style_text_font(_unitsLabel, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(_unitsLabel, COLOR_TEXT_PRIMARY, 0);
    lv_obj_align(_unitsLabel, LV_ALIGN_TOP_RIGHT, 0, 42);

    // Row 2: Switches (y=62)
    _soundSwitch = lv_switch_create(_settingsContainer);
    lv_obj_set_size(_soundSwitch, 50, 24);
    lv_obj_set_pos(_soundSwitch, 10, 66);  // 10px left padding
    lv_obj_set_style_bg_color(_soundSwitch, COLOR_INACTIVE, LV_PART_MAIN);
    lv_obj_set_style_bg_color(_soundSwitch, COLOR_ACTIVE, LV_PART_INDICATOR | LV_STATE_CHECKED);
    if (_settings.soundSetting) {
        lv_obj_add_state(_soundSwitch, LV_STATE_CHECKED);
    }
    lv_obj_add_event_cb(_soundSwitch, _soundSwitchHandler, LV_EVENT_VALUE_CHANGED, this);

    _unitsSwitch = lv_switch_create(_settingsContainer);
    lv_obj_set_size(_unitsSwitch, 50, 24);
    lv_obj_align(_unitsSwitch, LV_ALIGN_TOP_RIGHT, -10, 66);  // 10px right padding
    lv_obj_set_style_bg_color(_unitsSwitch, COLOR_INACTIVE, LV_PART_MAIN);
    lv_obj_set_style_bg_color(_unitsSwitch, COLOR_ACTIVE, LV_PART_INDICATOR | LV_STATE_CHECKED);
    if (_settings.imperialUnitsSetting) {
        lv_obj_add_state(_unitsSwitch, LV_STATE_CHECKED);
    }
    lv_obj_add_event_cb(_unitsSwitch, _unitsSwitchHandler, LV_EVENT_VALUE_CHANGED, this);
}

void SettingsScreen::_createActionButtons() {
    // Delete All Trips button - positioned after settings container
    _deleteAllButton = lv_button_create(_contentPane);
    lv_obj_set_size(_deleteAllButton, CONTENT_PANE_WIDTH - 32, 34);
    lv_obj_set_pos(_deleteAllButton, 8, 150);
    lv_obj_set_style_bg_color(_deleteAllButton, lv_color_hex(0x8B4513), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(_deleteAllButton, lv_color_hex(0xA0522D), LV_STATE_PRESSED);
    lv_obj_set_style_radius(_deleteAllButton, 4, 0);

    lv_obj_t* deleteLabel = lv_label_create(_deleteAllButton);
    lv_label_set_text(deleteLabel, LV_SYMBOL_TRASH " Delete All Trips");
    lv_obj_set_style_text_font(deleteLabel, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(deleteLabel, COLOR_TEXT_PRIMARY, 0);
    lv_obj_center(deleteLabel);

    lv_obj_add_event_cb(_deleteAllButton, _deleteAllButtonHandler, LV_EVENT_CLICKED, this);

    // Factory Reset button
    _factoryResetButton = lv_button_create(_contentPane);
    lv_obj_set_size(_factoryResetButton, CONTENT_PANE_WIDTH - 32, 34);
    lv_obj_set_pos(_factoryResetButton, 8, 190);
    lv_obj_set_style_bg_color(_factoryResetButton, COLOR_BATTERY_CRITICAL, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(_factoryResetButton, lv_color_hex(0xFF5252), LV_STATE_PRESSED);
    lv_obj_set_style_radius(_factoryResetButton, 4, 0);

    lv_obj_t* resetLabel = lv_label_create(_factoryResetButton);
    lv_label_set_text(resetLabel, LV_SYMBOL_WARNING " Factory Reset");
    lv_obj_set_style_text_font(resetLabel, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(resetLabel, COLOR_TEXT_PRIMARY, 0);
    lv_obj_center(resetLabel);

    lv_obj_add_event_cb(_factoryResetButton, _factoryResetButtonHandler, LV_EVENT_CLICKED, this);
}

void SettingsScreen::_showDeleteAllConfirmation() {
    _pendingAction = 1;

    _msgbox = lv_msgbox_create(NULL);
    lv_msgbox_add_title(_msgbox, "Confirm Delete");
    lv_msgbox_add_text(_msgbox, "Delete all trip data?\nThis cannot be undone.");

    lv_obj_t* btnYes = lv_msgbox_add_footer_button(_msgbox, "Yes");
    lv_obj_t* btnNo = lv_msgbox_add_footer_button(_msgbox, "No");

    lv_obj_add_event_cb(btnYes, _msgboxHandler, LV_EVENT_CLICKED, this);
    lv_obj_add_event_cb(btnNo, _msgboxHandler, LV_EVENT_CLICKED, this);

    lv_obj_center(_msgbox);

    lv_obj_set_style_bg_color(_msgbox, lv_color_hex(0x404040), 0);
    lv_obj_set_style_text_color(_msgbox, COLOR_TEXT_PRIMARY, 0);
}

void SettingsScreen::_showFactoryResetConfirmation() {
    _pendingAction = 2;

    _msgbox = lv_msgbox_create(NULL);
    lv_msgbox_add_title(_msgbox, "Factory Reset");
    lv_msgbox_add_text(_msgbox, "Reset all settings and\ndelete all data?");

    lv_obj_t* btnYes = lv_msgbox_add_footer_button(_msgbox, "Yes");
    lv_obj_t* btnNo = lv_msgbox_add_footer_button(_msgbox, "No");

    lv_obj_add_event_cb(btnYes, _msgboxHandler, LV_EVENT_CLICKED, this);
    lv_obj_add_event_cb(btnNo, _msgboxHandler, LV_EVENT_CLICKED, this);

    lv_obj_center(_msgbox);

    lv_obj_set_style_bg_color(_msgbox, lv_color_hex(0x404040), 0);
    lv_obj_set_style_text_color(_msgbox, COLOR_TEXT_PRIMARY, 0);
}

void SettingsScreen::_deleteAllTrips() {
    // Delete all GPX files from /rides/ directory
    File ridesDir = SD.open("/rides");
    if (ridesDir && ridesDir.isDirectory()) {
        File file = ridesDir.openNextFile();
        while (file) {
            if (!file.isDirectory()) {
                String filename = file.name();
                if (filename.endsWith(".gpx")) {
                    String fullPath = "/rides/" + filename;
                    SD.remove(fullPath.c_str());
                }
            }
            file = ridesDir.openNextFile();
        }
        ridesDir.close();
    }

    Serial.println(F("All trips deleted"));
}

void SettingsScreen::_performFactoryReset() {
    // Delete all trips
    _deleteAllTrips();

    // Reset settings to defaults
    if (_settingsUtils != nullptr) {
        _settings = _settingsUtils->resetSettings();
    }

    // Update UI to reflect reset settings
    if (_soundSwitch != nullptr) {
        lv_obj_remove_state(_soundSwitch, LV_STATE_CHECKED);
    }
    if (_unitsSwitch != nullptr) {
        lv_obj_remove_state(_unitsSwitch, LV_STATE_CHECKED);
        lv_label_set_text(_unitsLabel, "km/h");
    }
    if (_wheelDropdown != nullptr) {
        lv_dropdown_set_selected(_wheelDropdown, 0);
    }

    Serial.println(F("Factory reset completed"));
}

// Event handlers
void SettingsScreen::_soundSwitchHandler(lv_event_t* e) {
    SettingsScreen* screen = static_cast<SettingsScreen*>(lv_event_get_user_data(e));
    lv_obj_t* sw = static_cast<lv_obj_t*>(lv_event_get_target(e));

    bool isChecked = lv_obj_has_state(sw, LV_STATE_CHECKED);
    screen->_settings.soundSetting = isChecked;

    if (screen->_settingsUtils != nullptr) {
        screen->_settingsUtils->storeSoundSetting(isChecked);
    }

    Serial.print(F("Sound setting: "));
    Serial.println(isChecked ? "ON" : "OFF");
}

void SettingsScreen::_unitsSwitchHandler(lv_event_t* e) {
    SettingsScreen* screen = static_cast<SettingsScreen*>(lv_event_get_user_data(e));
    lv_obj_t* sw = static_cast<lv_obj_t*>(lv_event_get_target(e));

    bool isChecked = lv_obj_has_state(sw, LV_STATE_CHECKED);
    screen->_settings.imperialUnitsSetting = isChecked;

    if (screen->_settingsUtils != nullptr) {
        screen->_settingsUtils->storeImperialUnitsSetting(isChecked);
    }

    // Update label to show current unit
    if (screen->_unitsLabel != nullptr) {
        lv_label_set_text(screen->_unitsLabel, isChecked ? "mph" : "km/h");
    }

    Serial.print(F("Imperial units: "));
    Serial.println(isChecked ? "ON" : "OFF");
}

void SettingsScreen::_wheelDropdownHandler(lv_event_t* e) {
    SettingsScreen* screen = static_cast<SettingsScreen*>(lv_event_get_user_data(e));
    lv_obj_t* dropdown = static_cast<lv_obj_t*>(lv_event_get_target(e));

    uint16_t selected = lv_dropdown_get_selected(dropdown);
    screen->_settings.wheelTyreIndex = selected;

    if (screen->_settingsUtils != nullptr) {
        screen->_settingsUtils->storeWheelTyreSetting(selected);
    }

    Serial.print(F("Wheel tyre index: "));
    Serial.println(selected);
}

void SettingsScreen::_deleteAllButtonHandler(lv_event_t* e) {
    SettingsScreen* screen = static_cast<SettingsScreen*>(lv_event_get_user_data(e));
    screen->_showDeleteAllConfirmation();
}

void SettingsScreen::_factoryResetButtonHandler(lv_event_t* e) {
    SettingsScreen* screen = static_cast<SettingsScreen*>(lv_event_get_user_data(e));
    screen->_showFactoryResetConfirmation();
}

void SettingsScreen::_msgboxHandler(lv_event_t* e) {
    SettingsScreen* screen = static_cast<SettingsScreen*>(lv_event_get_user_data(e));
    lv_obj_t* btn = static_cast<lv_obj_t*>(lv_event_get_target(e));

    // Get which button was clicked by getting its label text
    lv_obj_t* btnLabel = lv_obj_get_child(btn, 0);
    const char* btnText = btnLabel != nullptr ? lv_label_get_text(btnLabel) : nullptr;

    if (btnText != nullptr && strcmp(btnText, "Yes") == 0) {
        if (screen->_pendingAction == 1) {
            screen->_deleteAllTrips();
        } else if (screen->_pendingAction == 2) {
            screen->_performFactoryReset();
        }
    }

    screen->_pendingAction = 0;
    if (screen->_msgbox != nullptr) {
        lv_msgbox_close(screen->_msgbox);
        screen->_msgbox = nullptr;
    }
}
