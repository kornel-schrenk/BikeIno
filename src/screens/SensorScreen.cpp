#include "screens/SensorScreen.h"

SensorScreen::SensorScreen() {
    _gps = nullptr;
    _hallActive = false;
    _hallPulseInterval = 0;
    _hallRpm = 0;

    _hallPane = nullptr;
    _hallIcon = nullptr;
    _hallRpmLabel = nullptr;
    _gpsPane = nullptr;
    _gpsIcon = nullptr;
    _gpsSpeedLabel = nullptr;
    _gpsLatLabel = nullptr;
    _gpsLngLabel = nullptr;
    _gpsEleLabel = nullptr;
}

void SensorScreen::setGps(TinyGPSPlus* gps) {
    _gps = gps;
}

void SensorScreen::setHallSensorData(bool active, unsigned long pulseInterval, unsigned int rpm) {
    _hallActive = active;
    _hallPulseInterval = pulseInterval;
    _hallRpm = rpm;
}

void SensorScreen::setSettings(BikeInoSettings settings) {
    _settings = settings;
}

void SensorScreen::createUI() {
    createBaseLayout();
    _createHallPane();
    _createGpsPane();
}

void SensorScreen::destroyUI() {
    BikeInoScreen::destroyUI();

    _hallPane = nullptr;
    _hallIcon = nullptr;
    _hallRpmLabel = nullptr;
    _gpsPane = nullptr;
    _gpsIcon = nullptr;
    _gpsSpeedLabel = nullptr;
    _gpsLatLabel = nullptr;
    _gpsLngLabel = nullptr;
    _gpsEleLabel = nullptr;
}

void SensorScreen::update() {
    if (!isCreated()) return;

    _updateHallPane();
    _updateGpsPane();
}

void SensorScreen::_createHallPane() {
    // Hall sensor pane (top, ~60px height)
    _hallPane = lv_obj_create(_contentPane);
    lv_obj_set_size(_hallPane, CONTENT_PANE_WIDTH - 16, 60);
    lv_obj_set_pos(_hallPane, 0, 0);
    lv_obj_set_style_bg_color(_hallPane, lv_color_hex(0x2A2A2A), 0);
    lv_obj_set_style_border_width(_hallPane, 0, 0);
    lv_obj_set_style_radius(_hallPane, 8, 0);
    lv_obj_set_style_pad_all(_hallPane, 8, 0);
    lv_obj_clear_flag(_hallPane, LV_OBJ_FLAG_SCROLLABLE);

    // Title
    lv_obj_t* title = lv_label_create(_hallPane);
    lv_label_set_text(title, "Hall Sensor");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(title, COLOR_TEXT_SECONDARY, 0);
    lv_obj_align(title, LV_ALIGN_TOP_LEFT, 0, 0);

    // Icon
    _hallIcon = lv_label_create(_hallPane);
    lv_label_set_text(_hallIcon, LV_SYMBOL_REFRESH);
    lv_obj_set_style_text_font(_hallIcon, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(_hallIcon, COLOR_INACTIVE, 0);
    lv_obj_align(_hallIcon, LV_ALIGN_LEFT_MID, 0, 8);

    // RPM label
    _hallRpmLabel = lv_label_create(_hallPane);
    lv_label_set_text(_hallRpmLabel, "RPM: N/A");
    lv_obj_set_style_text_font(_hallRpmLabel, &lv_font_montserrat_18, 0);
    lv_obj_set_style_text_color(_hallRpmLabel, COLOR_TEXT_PRIMARY, 0);
    lv_obj_align(_hallRpmLabel, LV_ALIGN_RIGHT_MID, 0, 8);
}

void SensorScreen::_createGpsPane() {
    // GPS sensor pane (bottom, remaining space ~164px)
    _gpsPane = lv_obj_create(_contentPane);
    lv_obj_set_size(_gpsPane, CONTENT_PANE_WIDTH - 16, 164);
    lv_obj_set_pos(_gpsPane, 0, 68);
    lv_obj_set_style_bg_color(_gpsPane, lv_color_hex(0x2A2A2A), 0);
    lv_obj_set_style_border_width(_gpsPane, 0, 0);
    lv_obj_set_style_radius(_gpsPane, 8, 0);
    lv_obj_set_style_pad_all(_gpsPane, 8, 0);
    lv_obj_clear_flag(_gpsPane, LV_OBJ_FLAG_SCROLLABLE);

    // Title
    lv_obj_t* title = lv_label_create(_gpsPane);
    lv_label_set_text(title, "GPS Sensor");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(title, COLOR_TEXT_SECONDARY, 0);
    lv_obj_align(title, LV_ALIGN_TOP_LEFT, 0, 0);

    // Icon
    _gpsIcon = lv_label_create(_gpsPane);
    lv_label_set_text(_gpsIcon, LV_SYMBOL_GPS);
    lv_obj_set_style_text_font(_gpsIcon, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(_gpsIcon, COLOR_INACTIVE, 0);
    lv_obj_align(_gpsIcon, LV_ALIGN_TOP_RIGHT, 0, 0);

    // Speed label
    _gpsSpeedLabel = lv_label_create(_gpsPane);
    lv_label_set_text(_gpsSpeedLabel, "Speed: -- km/h");
    lv_obj_set_style_text_font(_gpsSpeedLabel, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_color(_gpsSpeedLabel, COLOR_TEXT_PRIMARY, 0);
    lv_obj_set_pos(_gpsSpeedLabel, 0, 28);

    // Latitude label
    _gpsLatLabel = lv_label_create(_gpsPane);
    lv_label_set_text(_gpsLatLabel, "Lat: --");
    lv_obj_set_style_text_font(_gpsLatLabel, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_color(_gpsLatLabel, COLOR_TEXT_PRIMARY, 0);
    lv_obj_set_pos(_gpsLatLabel, 0, 52);

    // Longitude label
    _gpsLngLabel = lv_label_create(_gpsPane);
    lv_label_set_text(_gpsLngLabel, "Lng: --");
    lv_obj_set_style_text_font(_gpsLngLabel, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_color(_gpsLngLabel, COLOR_TEXT_PRIMARY, 0);
    lv_obj_set_pos(_gpsLngLabel, 0, 76);

    // Elevation label
    _gpsEleLabel = lv_label_create(_gpsPane);
    lv_label_set_text(_gpsEleLabel, "Ele: -- m");
    lv_obj_set_style_text_font(_gpsEleLabel, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_color(_gpsEleLabel, COLOR_TEXT_PRIMARY, 0);
    lv_obj_set_pos(_gpsEleLabel, 0, 100);
}

void SensorScreen::_updateHallPane() {
    if (_hallIcon == nullptr || _hallRpmLabel == nullptr) return;

    // Update icon color
    lv_obj_set_style_text_color(_hallIcon, _hallActive ? COLOR_ACTIVE : COLOR_INACTIVE, 0);

    // Update RPM
    char buf[32];
    if (_hallActive && _hallRpm > 0) {
        snprintf(buf, sizeof(buf), "RPM: %d", _hallRpm);
    } else {
        snprintf(buf, sizeof(buf), "RPM: N/A");
    }
    lv_label_set_text(_hallRpmLabel, buf);
}

void SensorScreen::_updateGpsPane() {
    if (_gpsIcon == nullptr) return;

    bool gpsValid = _gps != nullptr && _gps->location.isValid();

    // Update icon color
    lv_obj_set_style_text_color(_gpsIcon, gpsValid ? COLOR_ACTIVE : COLOR_INACTIVE, 0);

    char buf[64];

    // Update speed
    if (_gps != nullptr && _gps->speed.isValid()) {
        double speed = _settings.imperialUnitsSetting ? _gps->speed.mph() : _gps->speed.kmph();
        snprintf(buf, sizeof(buf), "Speed: %.1f %s", speed, _settings.imperialUnitsSetting ? "mph" : "km/h");
    } else {
        snprintf(buf, sizeof(buf), "Speed: --");
    }
    lv_label_set_text(_gpsSpeedLabel, buf);

    // Update latitude
    if (gpsValid) {
        snprintf(buf, sizeof(buf), "Lat: %.6f", _gps->location.lat());
    } else {
        snprintf(buf, sizeof(buf), "Lat: No GPS Fix");
    }
    lv_label_set_text(_gpsLatLabel, buf);

    // Update longitude
    if (gpsValid) {
        snprintf(buf, sizeof(buf), "Lng: %.6f", _gps->location.lng());
    } else {
        snprintf(buf, sizeof(buf), "Lng: --");
    }
    lv_label_set_text(_gpsLngLabel, buf);

    // Update elevation
    if (_gps != nullptr && _gps->altitude.isValid()) {
        double ele = _settings.imperialUnitsSetting ? _gps->altitude.feet() : _gps->altitude.meters();
        snprintf(buf, sizeof(buf), "Ele: %.1f %s", ele, _settings.imperialUnitsSetting ? "ft" : "m");
    } else {
        snprintf(buf, sizeof(buf), "Ele: --");
    }
    lv_label_set_text(_gpsEleLabel, buf);
}
