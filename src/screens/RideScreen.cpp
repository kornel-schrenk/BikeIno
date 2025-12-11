#include "screens/RideScreen.h"

RideScreen::RideScreen(BikeRide* bikeRide) {
    _bikeRide = bikeRide;
    _gps = nullptr;

    _gpsEnabled = true;
    _hallEnabled = false;
    _hallActive = false;
    _hallPulseInterval = 0;

    // Initialize pointers
    _topPane = nullptr;
    _gpsButton = nullptr;
    _gpsIcon = nullptr;
    _hallButton = nullptr;
    _hallIcon = nullptr;
    _middlePane = nullptr;
    _speedLabel = nullptr;
    _speedUnitLabel = nullptr;
    _bottomPane = nullptr;
    _timeLabel = nullptr;
    _temperatureLabel = nullptr;
    _totalDistanceLabel = nullptr;
    _tripDurationLabel = nullptr;
    _tripDistanceLabel = nullptr;
    _startButton = nullptr;
    _pauseButton = nullptr;
    _stopButton = nullptr;
}

void RideScreen::setGps(TinyGPSPlus* gps) {
    _gps = gps;
}

void RideScreen::setHallSensorData(bool active, unsigned long pulseInterval) {
    _hallActive = active;
    _hallPulseInterval = pulseInterval;
}

void RideScreen::setSettings(BikeInoSettings settings) {
    _settings = settings;
}

void RideScreen::createUI() {
    // Create base layout (screen + content pane)
    createBaseLayout();

    // Create the three panes
    _createTopPane();
    _createMiddlePane();
    _createBottomPane();
}

void RideScreen::destroyUI() {
    // Base class handles screen deletion
    BikeInoScreen::destroyUI();

    // Reset all pointers
    _topPane = nullptr;
    _gpsButton = nullptr;
    _gpsIcon = nullptr;
    _hallButton = nullptr;
    _hallIcon = nullptr;
    _middlePane = nullptr;
    _speedLabel = nullptr;
    _speedUnitLabel = nullptr;
    _bottomPane = nullptr;
    _timeLabel = nullptr;
    _temperatureLabel = nullptr;
    _totalDistanceLabel = nullptr;
    _tripDurationLabel = nullptr;
    _tripDistanceLabel = nullptr;
    _startButton = nullptr;
    _pauseButton = nullptr;
    _stopButton = nullptr;
}

void RideScreen::update() {
    if (!isCreated()) return;

    _updateSensorIcons();
    _updateSpeedDisplay();
    _updateInfoTiles();
    _updateRideControls();
}

void RideScreen::_createTopPane() {
    // Top pane container - no flex, manual positioning
    _topPane = lv_obj_create(_contentPane);
    lv_obj_set_size(_topPane, CONTENT_PANE_WIDTH - 16, RIDE_TOP_PANE_HEIGHT);
    lv_obj_set_pos(_topPane, 0, 0);
    lv_obj_set_style_bg_color(_topPane, COLOR_BACKGROUND, 0);
    lv_obj_set_style_border_width(_topPane, 0, 0);
    lv_obj_set_style_pad_all(_topPane, 0, 0);
    lv_obj_clear_flag(_topPane, LV_OBJ_FLAG_SCROLLABLE);

    // Left side: Ride control buttons - bigger size
    _startButton = lv_button_create(_topPane);
    lv_obj_set_size(_startButton, 60, 28);
    lv_obj_set_pos(_startButton, 0, 1);
    lv_obj_set_style_bg_color(_startButton, COLOR_ACTIVE, 0);
    lv_obj_set_style_radius(_startButton, 4, 0);
    lv_obj_t* startLabel = lv_label_create(_startButton);
    lv_label_set_text(startLabel, LV_SYMBOL_PLAY);
    lv_obj_set_style_text_font(startLabel, &lv_font_montserrat_16, 0);
    lv_obj_center(startLabel);
    lv_obj_add_event_cb(_startButton, _startButtonHandler, LV_EVENT_CLICKED, this);

    _pauseButton = lv_button_create(_topPane);
    lv_obj_set_size(_pauseButton, 60, 28);
    lv_obj_set_pos(_pauseButton, 0, 1);
    lv_obj_set_style_bg_color(_pauseButton, lv_color_hex(0xFF9800), 0);
    lv_obj_set_style_radius(_pauseButton, 4, 0);
    lv_obj_t* pauseLabel = lv_label_create(_pauseButton);
    lv_label_set_text(pauseLabel, LV_SYMBOL_PAUSE);
    lv_obj_set_style_text_font(pauseLabel, &lv_font_montserrat_16, 0);
    lv_obj_center(pauseLabel);
    lv_obj_add_event_cb(_pauseButton, _pauseButtonHandler, LV_EVENT_CLICKED, this);
    lv_obj_add_flag(_pauseButton, LV_OBJ_FLAG_HIDDEN);

    _stopButton = lv_button_create(_topPane);
    lv_obj_set_size(_stopButton, 60, 28);
    lv_obj_set_pos(_stopButton, 64, 1);
    lv_obj_set_style_bg_color(_stopButton, lv_color_hex(0xE53935), 0);
    lv_obj_set_style_radius(_stopButton, 4, 0);
    lv_obj_t* stopLabel = lv_label_create(_stopButton);
    lv_label_set_text(stopLabel, LV_SYMBOL_STOP);
    lv_obj_set_style_text_font(stopLabel, &lv_font_montserrat_16, 0);
    lv_obj_center(stopLabel);
    lv_obj_add_event_cb(_stopButton, _stopButtonHandler, LV_EVENT_CLICKED, this);
    lv_obj_add_flag(_stopButton, LV_OBJ_FLAG_HIDDEN);

    // Right side: Sensor icons
    _hallButton = lv_button_create(_topPane);
    lv_obj_set_size(_hallButton, 40, 28);
    lv_obj_align(_hallButton, LV_ALIGN_TOP_RIGHT, 0, 1);
    lv_obj_set_style_bg_color(_hallButton, COLOR_BACKGROUND, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(_hallButton, lv_color_hex(0x404040), LV_STATE_PRESSED);
    lv_obj_set_style_border_width(_hallButton, 1, 0);
    lv_obj_set_style_border_color(_hallButton, COLOR_INACTIVE, 0);
    lv_obj_set_style_radius(_hallButton, 4, 0);

    _hallIcon = lv_label_create(_hallButton);
    lv_label_set_text(_hallIcon, LV_SYMBOL_REFRESH);
    lv_obj_set_style_text_font(_hallIcon, &lv_font_montserrat_16, 0);
    lv_obj_center(_hallIcon);
    lv_obj_add_event_cb(_hallButton, _hallButtonHandler, LV_EVENT_CLICKED, this);

    _gpsButton = lv_button_create(_topPane);
    lv_obj_set_size(_gpsButton, 40, 28);
    lv_obj_align(_gpsButton, LV_ALIGN_TOP_RIGHT, -44, 1);
    lv_obj_set_style_bg_color(_gpsButton, COLOR_BACKGROUND, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(_gpsButton, lv_color_hex(0x404040), LV_STATE_PRESSED);
    lv_obj_set_style_border_width(_gpsButton, 1, 0);
    lv_obj_set_style_border_color(_gpsButton, COLOR_INACTIVE, 0);
    lv_obj_set_style_radius(_gpsButton, 4, 0);

    _gpsIcon = lv_label_create(_gpsButton);
    lv_label_set_text(_gpsIcon, LV_SYMBOL_GPS);
    lv_obj_set_style_text_font(_gpsIcon, &lv_font_montserrat_16, 0);
    lv_obj_center(_gpsIcon);
    lv_obj_add_event_cb(_gpsButton, _gpsButtonHandler, LV_EVENT_CLICKED, this);

    _updateSensorIcons();
}

void RideScreen::_createMiddlePane() {
    // Middle pane container - centered speed display
    _middlePane = lv_obj_create(_contentPane);
    lv_obj_set_size(_middlePane, CONTENT_PANE_WIDTH - 16, RIDE_MIDDLE_PANE_HEIGHT);
    lv_obj_set_pos(_middlePane, 0, RIDE_TOP_PANE_HEIGHT + 4);
    lv_obj_set_style_bg_color(_middlePane, COLOR_BACKGROUND, 0);
    lv_obj_set_style_border_width(_middlePane, 0, 0);
    lv_obj_set_style_pad_all(_middlePane, 0, 0);
    lv_obj_clear_flag(_middlePane, LV_OBJ_FLAG_SCROLLABLE);

    // Speed label (large)
    _speedLabel = lv_label_create(_middlePane);
    lv_label_set_text(_speedLabel, "0.0");
    lv_obj_set_style_text_font(_speedLabel, &lv_font_montserrat_48, 0);
    lv_obj_set_style_text_color(_speedLabel, COLOR_TEXT_PRIMARY, 0);
    lv_obj_align(_speedLabel, LV_ALIGN_CENTER, 0, -8);

    // Speed unit label
    _speedUnitLabel = lv_label_create(_middlePane);
    lv_label_set_text(_speedUnitLabel, _settings.imperialUnitsSetting ? "mph" : "km/h");
    lv_obj_set_style_text_font(_speedUnitLabel, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_color(_speedUnitLabel, COLOR_TEXT_SECONDARY, 0);
    lv_obj_align(_speedUnitLabel, LV_ALIGN_CENTER, 0, 32);
}

void RideScreen::_createBottomPane() {
    // Bottom pane container - scrollable, one full-width tile visible at a time
    _bottomPane = lv_obj_create(_contentPane);
    int tileWidth = CONTENT_PANE_WIDTH - 16;
    lv_obj_set_size(_bottomPane, tileWidth, RIDE_BOTTOM_PANE_HEIGHT);
    lv_obj_set_pos(_bottomPane, 0, RIDE_TOP_PANE_HEIGHT + RIDE_MIDDLE_PANE_HEIGHT + 8);
    lv_obj_set_style_bg_color(_bottomPane, lv_color_hex(0x2A2A2A), 0);
    lv_obj_set_style_border_width(_bottomPane, 0, 0);
    lv_obj_set_style_pad_all(_bottomPane, 0, 0);
    lv_obj_set_style_radius(_bottomPane, 8, 0);

    // Enable horizontal scrolling with snap
    lv_obj_set_scroll_dir(_bottomPane, LV_DIR_HOR);
    lv_obj_set_scroll_snap_x(_bottomPane, LV_SCROLL_SNAP_CENTER);
    lv_obj_set_scrollbar_mode(_bottomPane, LV_SCROLLBAR_MODE_OFF);

    // Use flex layout for tiles
    lv_obj_set_flex_flow(_bottomPane, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(_bottomPane, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    // Helper lambda to create full-width info tiles
    auto createTile = [this, tileWidth](const char* title, const char* initialValue) -> lv_obj_t* {
        lv_obj_t* tile = lv_obj_create(_bottomPane);
        lv_obj_set_size(tile, tileWidth - 8, RIDE_BOTTOM_PANE_HEIGHT - 8);
        lv_obj_set_style_bg_color(tile, lv_color_hex(0x383838), 0);
        lv_obj_set_style_border_width(tile, 0, 0);
        lv_obj_set_style_radius(tile, 4, 0);
        lv_obj_set_style_pad_all(tile, 4, 0);
        lv_obj_clear_flag(tile, LV_OBJ_FLAG_SCROLLABLE);

        lv_obj_t* titleLabel = lv_label_create(tile);
        lv_label_set_text(titleLabel, title);
        lv_obj_set_style_text_font(titleLabel, &lv_font_montserrat_12, 0);
        lv_obj_set_style_text_color(titleLabel, COLOR_TEXT_SECONDARY, 0);
        lv_obj_align(titleLabel, LV_ALIGN_TOP_MID, 0, 2);

        lv_obj_t* valueLabel = lv_label_create(tile);
        lv_label_set_text(valueLabel, initialValue);
        lv_obj_set_style_text_font(valueLabel, &lv_font_montserrat_20, 0);
        lv_obj_set_style_text_color(valueLabel, COLOR_TEXT_PRIMARY, 0);
        lv_obj_align(valueLabel, LV_ALIGN_CENTER, 0, 6);

        return valueLabel;
    };

    // Create full-width tiles (swipe to see each one)
    _timeLabel = createTile("Current Time", "--:--");
    _temperatureLabel = createTile("Temperature", "--" "\xC2\xB0" "C");
    _totalDistanceLabel = createTile("Total Distance", "0.0 km");
    _tripDurationLabel = createTile("Trip Duration", "00:00:00");
    _tripDistanceLabel = createTile("Trip Distance", "0.0 km");
}

void RideScreen::_updateSensorIcons() {
    if (_gpsIcon == nullptr || _hallIcon == nullptr) return;

    // GPS icon color
    bool gpsValid = _gps != nullptr && _gps->location.isValid();
    lv_color_t gpsColor = _gpsEnabled ? (gpsValid ? COLOR_ACTIVE : lv_color_hex(0xFFEB3B)) : COLOR_INACTIVE;
    lv_obj_set_style_text_color(_gpsIcon, gpsColor, 0);
    lv_obj_set_style_border_color(_gpsButton, _gpsEnabled ? COLOR_ACTIVE : COLOR_INACTIVE, 0);

    // Hall icon color
    lv_color_t hallColor = _hallEnabled ? (_hallActive ? COLOR_ACTIVE : lv_color_hex(0xFFEB3B)) : COLOR_INACTIVE;
    lv_obj_set_style_text_color(_hallIcon, hallColor, 0);
    lv_obj_set_style_border_color(_hallButton, _hallEnabled ? COLOR_ACTIVE : COLOR_INACTIVE, 0);
}

void RideScreen::_updateSpeedDisplay() {
    if (_speedLabel == nullptr) return;

    double speed = 0.0;

    // Hall sensor takes priority if enabled and active
    if (_hallEnabled && _hallActive) {
        speed = _calculateSpeedFromHall();
    } else if (_gpsEnabled && _gps != nullptr && _gps->speed.isValid()) {
        speed = _settings.imperialUnitsSetting ? _gps->speed.mph() : _gps->speed.kmph();
    }

    char buf[16];
    snprintf(buf, sizeof(buf), "%.1f", speed);
    lv_label_set_text(_speedLabel, buf);

    // Update unit label
    lv_label_set_text(_speedUnitLabel, _settings.imperialUnitsSetting ? "mph" : "km/h");
}

void RideScreen::_updateInfoTiles() {
    if (_timeLabel == nullptr) return;

    char buf[32];

    // Time from GPS
    if (_gps != nullptr && _gps->time.isValid()) {
        snprintf(buf, sizeof(buf), "%02d:%02d", _gps->time.hour(), _gps->time.minute());
        lv_label_set_text(_timeLabel, buf);
    }

    // Temperature (placeholder - M5Stack doesn't have built-in temp sensor)
    // Could be added from external sensor in future

    // Total distance (from BikeRide)
    if (_bikeRide != nullptr) {
        double totalDist = _bikeRide->getDistance();
        if (_settings.imperialUnitsSetting) {
            totalDist *= 0.621371; // km to miles
            snprintf(buf, sizeof(buf), "%.1f mi", totalDist);
        } else {
            snprintf(buf, sizeof(buf), "%.1f km", totalDist);
        }
        lv_label_set_text(_totalDistanceLabel, buf);

        // Trip duration
        unsigned int duration = _bikeRide->getDuration();
        unsigned int hours = duration / 3600;
        unsigned int minutes = (duration % 3600) / 60;
        unsigned int seconds = duration % 60;
        snprintf(buf, sizeof(buf), "%02d:%02d:%02d", hours, minutes, seconds);
        lv_label_set_text(_tripDurationLabel, buf);

        // Trip distance (same as total for now)
        double tripDist = _bikeRide->getDistance();
        if (_settings.imperialUnitsSetting) {
            tripDist *= 0.621371;
            snprintf(buf, sizeof(buf), "%.1f mi", tripDist);
        } else {
            snprintf(buf, sizeof(buf), "%.1f km", tripDist);
        }
        lv_label_set_text(_tripDistanceLabel, buf);
    }
}

void RideScreen::_updateRideControls() {
    if (_startButton == nullptr || _pauseButton == nullptr || _stopButton == nullptr) return;

    bool inProgress = _bikeRide != nullptr && _bikeRide->isRideInProgress();
    bool isPaused = _bikeRide != nullptr && _bikeRide->isRidePaused();

    if (!inProgress) {
        // Show start button only
        lv_obj_clear_flag(_startButton, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(_pauseButton, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(_stopButton, LV_OBJ_FLAG_HIDDEN);
    } else {
        // Show pause and stop buttons
        lv_obj_add_flag(_startButton, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(_pauseButton, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(_stopButton, LV_OBJ_FLAG_HIDDEN);

        // Update pause button to show play if paused
        lv_obj_t* pauseLabel = lv_obj_get_child(_pauseButton, 0);
        if (pauseLabel != nullptr) {
            lv_label_set_text(pauseLabel, isPaused ? LV_SYMBOL_PLAY : LV_SYMBOL_PAUSE);
        }
    }
}

double RideScreen::_calculateSpeedFromHall() {
    if (_hallPulseInterval == 0) return 0.0;

    // Get wheel circumference from settings (default to 700x25C)
    uint16_t circumference = WHEEL_CIRCUMFERENCES[_settings.imperialUnitsSetting ? 4 : 4]; // TODO: use actual wheel setting

    // Calculate speed: circumference (mm) / interval (us) * 3600000 = km/h
    // circumference in mm, interval in microseconds
    double speedKmh = (double)circumference / (double)_hallPulseInterval * 3600.0;

    if (_settings.imperialUnitsSetting) {
        return speedKmh * 0.621371;
    }
    return speedKmh;
}

// Event handlers
void RideScreen::_gpsButtonHandler(lv_event_t* e) {
    RideScreen* screen = static_cast<RideScreen*>(lv_event_get_user_data(e));
    if (screen != nullptr) {
        screen->_gpsEnabled = !screen->_gpsEnabled;
        screen->_updateSensorIcons();
    }
}

void RideScreen::_hallButtonHandler(lv_event_t* e) {
    RideScreen* screen = static_cast<RideScreen*>(lv_event_get_user_data(e));
    if (screen != nullptr) {
        screen->_hallEnabled = !screen->_hallEnabled;
        screen->_updateSensorIcons();
    }
}

void RideScreen::_startButtonHandler(lv_event_t* e) {
    RideScreen* screen = static_cast<RideScreen*>(lv_event_get_user_data(e));
    if (screen != nullptr && screen->_bikeRide != nullptr && screen->_gps != nullptr) {
        screen->_bikeRide->startRide(
            screen->_gps->speed.kmph(),
            screen->_gps->location.lat(),
            screen->_gps->location.lng(),
            screen->_gps->altitude.meters()
        );
        screen->_updateRideControls();
    }
}

void RideScreen::_pauseButtonHandler(lv_event_t* e) {
    RideScreen* screen = static_cast<RideScreen*>(lv_event_get_user_data(e));
    if (screen != nullptr && screen->_bikeRide != nullptr) {
        if (screen->_bikeRide->isRidePaused()) {
            screen->_bikeRide->restartRide();
        } else {
            screen->_bikeRide->pauseRide();
        }
        screen->_updateRideControls();
    }
}

void RideScreen::_stopButtonHandler(lv_event_t* e) {
    RideScreen* screen = static_cast<RideScreen*>(lv_event_get_user_data(e));
    if (screen != nullptr && screen->_bikeRide != nullptr) {
        screen->_bikeRide->stopRide();
        screen->_updateRideControls();
        // TODO: Navigate to summary screen
    }
}
