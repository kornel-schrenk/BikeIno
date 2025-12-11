#include "ui/NavigationPane.h"
#include <cstdio>

NavigationPane::NavigationPane() {
    _pane = nullptr;
    _batteryIcon = nullptr;
    _batteryLabel = nullptr;
    _navCallback = nullptr;
    _activeScreenId = SCREEN_RIDE;

    for (int i = 0; i < 5; i++) {
        _screenIcons[i] = nullptr;
    }
}

void NavigationPane::create(lv_obj_t* parent) {
    // Create the navigation pane container
    _pane = lv_obj_create(parent);
    lv_obj_set_size(_pane, NAV_PANE_WIDTH, SCREEN_HEIGHT);
    lv_obj_set_pos(_pane, CONTENT_PANE_WIDTH, 0);
    lv_obj_set_style_bg_color(_pane, COLOR_NAV_BACKGROUND, 0);
    lv_obj_set_style_border_width(_pane, 0, 0);
    lv_obj_set_style_radius(_pane, 0, 0);
    lv_obj_set_style_pad_all(_pane, 0, 0);
    lv_obj_clear_flag(_pane, LV_OBJ_FLAG_SCROLLABLE);

    _createBatteryIndicator();
    _createScreenIcons();
}

void NavigationPane::destroy() {
    if (_pane != nullptr) {
        lv_obj_delete(_pane);
        _pane = nullptr;
        _batteryIcon = nullptr;
        _batteryLabel = nullptr;
        for (int i = 0; i < 5; i++) {
            _screenIcons[i] = nullptr;
        }
    }
}

void NavigationPane::_createBatteryIndicator() {
    // Battery container at top
    lv_obj_t* batteryContainer = lv_obj_create(_pane);
    lv_obj_set_size(batteryContainer, NAV_PANE_WIDTH, 40);
    lv_obj_set_pos(batteryContainer, 0, 0);
    lv_obj_set_style_bg_color(batteryContainer, COLOR_NAV_BACKGROUND, 0);
    lv_obj_set_style_border_width(batteryContainer, 0, 0);
    lv_obj_set_style_pad_all(batteryContainer, 4, 0);
    lv_obj_clear_flag(batteryContainer, LV_OBJ_FLAG_SCROLLABLE);

    // Battery icon - larger to match text proportionally
    _batteryIcon = lv_label_create(batteryContainer);
    lv_label_set_text(_batteryIcon, LV_SYMBOL_BATTERY_FULL);
    lv_obj_set_style_text_font(_batteryIcon, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(_batteryIcon, COLOR_BATTERY_CHARGED, 0);
    lv_obj_align(_batteryIcon, LV_ALIGN_TOP_MID, 0, 0);

    // Battery percentage label - smaller text below icon
    _batteryLabel = lv_label_create(batteryContainer);
    lv_label_set_text(_batteryLabel, "100%");
    lv_obj_set_style_text_font(_batteryLabel, &lv_font_montserrat_10, 0);
    lv_obj_set_style_text_color(_batteryLabel, COLOR_TEXT_SECONDARY, 0);
    lv_obj_align(_batteryLabel, LV_ALIGN_BOTTOM_MID, 0, 0);
}

void NavigationPane::_createScreenIcons() {
    // Screen icons symbols
    const char* symbols[] = {
        LV_SYMBOL_HOME,      // Ride
        LV_SYMBOL_EYE_OPEN,  // Sensor
        LV_SYMBOL_LIST,      // Logbook
        LV_SYMBOL_SHUFFLE,   // Summary (using shuffle as chart alternative)
        LV_SYMBOL_SETTINGS   // Settings
    };

    // Screen IDs corresponding to each icon
    const int screenIds[] = {
        SCREEN_RIDE,
        SCREEN_SENSOR,
        SCREEN_LOGBOOK,
        SCREEN_SUMMARY,
        SCREEN_SETTINGS
    };

    // Icon container (below battery)
    lv_obj_t* iconContainer = lv_obj_create(_pane);
    lv_obj_set_size(iconContainer, NAV_PANE_WIDTH, SCREEN_HEIGHT - 40);
    lv_obj_set_pos(iconContainer, 0, 40);
    lv_obj_set_style_bg_color(iconContainer, COLOR_NAV_BACKGROUND, 0);
    lv_obj_set_style_border_width(iconContainer, 0, 0);
    lv_obj_set_style_pad_all(iconContainer, 0, 0);
    lv_obj_clear_flag(iconContainer, LV_OBJ_FLAG_SCROLLABLE);

    // Use flex layout for icons
    lv_obj_set_flex_flow(iconContainer, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(iconContainer, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    for (int i = 0; i < 5; i++) {
        // Create button for each icon - reduced left/right padding
        lv_obj_t* btn = lv_button_create(iconContainer);
        lv_obj_set_size(btn, NAV_PANE_WIDTH - 6, 32);
        lv_obj_set_style_bg_color(btn, COLOR_NAV_BACKGROUND, LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(btn, COLOR_ACTIVE, LV_STATE_PRESSED);
        lv_obj_set_style_border_width(btn, 0, 0);
        lv_obj_set_style_radius(btn, 4, 0);
        lv_obj_set_style_shadow_width(btn, 0, 0);

        // Create icon label
        lv_obj_t* iconLabel = lv_label_create(btn);
        lv_label_set_text(iconLabel, symbols[i]);
        lv_obj_set_style_text_font(iconLabel, &lv_font_montserrat_20, 0);
        lv_obj_center(iconLabel);

        // Store screen ID in user data
        lv_obj_set_user_data(btn, (void*)(intptr_t)screenIds[i]);

        // Add click event
        lv_obj_add_event_cb(btn, _iconClickHandler, LV_EVENT_CLICKED, this);

        _screenIcons[i] = btn;
    }

    _updateIconStyles();
}

void NavigationPane::_updateIconStyles() {
    const int screenIds[] = {
        SCREEN_RIDE,
        SCREEN_SENSOR,
        SCREEN_LOGBOOK,
        SCREEN_SUMMARY,
        SCREEN_SETTINGS
    };

    for (int i = 0; i < 5; i++) {
        if (_screenIcons[i] == nullptr) continue;

        lv_obj_t* iconLabel = lv_obj_get_child(_screenIcons[i], 0);
        if (iconLabel == nullptr) continue;

        if (screenIds[i] == _activeScreenId) {
            lv_obj_set_style_text_color(iconLabel, COLOR_ACTIVE, 0);
            lv_obj_set_style_bg_color(_screenIcons[i], lv_color_hex(0x404040), 0);
        } else {
            lv_obj_set_style_text_color(iconLabel, COLOR_INACTIVE, 0);
            lv_obj_set_style_bg_color(_screenIcons[i], COLOR_NAV_BACKGROUND, 0);
        }
    }
}

void NavigationPane::setActiveScreen(int screenId) {
    _activeScreenId = screenId;
    _updateIconStyles();
}

void NavigationPane::updateBattery(int percent) {
    if (_batteryIcon == nullptr || _batteryLabel == nullptr) return;

    // Update percentage label
    char buf[8];
    snprintf(buf, sizeof(buf), "%d%%", percent);
    lv_label_set_text(_batteryLabel, buf);

    // Update icon and color based on level
    const char* symbol;
    lv_color_t color;

    if (percent <= 10) {
        symbol = LV_SYMBOL_BATTERY_EMPTY;
        color = COLOR_BATTERY_CRITICAL;
    } else if (percent <= 25) {
        symbol = LV_SYMBOL_BATTERY_1;
        color = COLOR_BATTERY_LOW;
    } else if (percent <= 50) {
        symbol = LV_SYMBOL_BATTERY_2;
        color = COLOR_BATTERY_NORMAL;
    } else if (percent <= 75) {
        symbol = LV_SYMBOL_BATTERY_3;
        color = COLOR_BATTERY_NORMAL;
    } else {
        symbol = LV_SYMBOL_BATTERY_FULL;
        color = COLOR_BATTERY_CHARGED;
    }

    lv_label_set_text(_batteryIcon, symbol);
    lv_obj_set_style_text_color(_batteryIcon, color, 0);
}

void NavigationPane::setNavigationCallback(NavigationCallback callback) {
    _navCallback = callback;
}

lv_obj_t* NavigationPane::getPane() {
    return _pane;
}

void NavigationPane::_iconClickHandler(lv_event_t* e) {
    lv_obj_t* target = static_cast<lv_obj_t*>(lv_event_get_target(e));
    NavigationPane* pane = static_cast<NavigationPane*>(lv_event_get_user_data(e));

    int screenId = (int)(intptr_t)lv_obj_get_user_data(target);

    if (pane != nullptr && pane->_navCallback != nullptr) {
        pane->_navCallback(screenId);
    }
}
