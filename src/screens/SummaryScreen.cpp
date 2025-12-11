#include "screens/SummaryScreen.h"
#include <SD.h>

SummaryScreen::SummaryScreen(BikeRide* bikeRide) {
    _bikeRide = bikeRide;

    _stats.totalDistance = 0;
    _stats.totalDuration = 0;
    _stats.tripCount = 0;
    _stats.avgDistance = 0;
    _stats.avgDuration = 0;
    _stats.avgSpeed = 0;

    _titleLabel = nullptr;
    _totalDistLabel = nullptr;
    _totalTimeLabel = nullptr;
    _tripCountLabel = nullptr;
    _avgDistLabel = nullptr;
    _avgTimeLabel = nullptr;
    _avgSpeedLabel = nullptr;
}

void SummaryScreen::setSettings(BikeInoSettings settings) {
    _settings = settings;
}

void SummaryScreen::createUI() {
    createBaseLayout();
    _loadStatistics();
    _createStatsList();
}

void SummaryScreen::destroyUI() {
    BikeInoScreen::destroyUI();

    _titleLabel = nullptr;
    _totalDistLabel = nullptr;
    _totalTimeLabel = nullptr;
    _tripCountLabel = nullptr;
    _avgDistLabel = nullptr;
    _avgTimeLabel = nullptr;
    _avgSpeedLabel = nullptr;
}

void SummaryScreen::update() {
    // Statistics are loaded once when screen is created
    // Could add periodic refresh if needed
}

void SummaryScreen::_createStatsList() {
    // Title
    _titleLabel = lv_label_create(_contentPane);
    lv_label_set_text(_titleLabel, "Statistics");
    lv_obj_set_style_text_font(_titleLabel, &lv_font_montserrat_18, 0);
    lv_obj_set_style_text_color(_titleLabel, COLOR_TEXT_PRIMARY, 0);
    lv_obj_align(_titleLabel, LV_ALIGN_TOP_MID, 0, 0);

    // Stats container
    lv_obj_t* statsContainer = lv_obj_create(_contentPane);
    lv_obj_set_size(statsContainer, CONTENT_PANE_WIDTH - 16, 200);
    lv_obj_set_pos(statsContainer, 0, 28);
    lv_obj_set_style_bg_color(statsContainer, lv_color_hex(0x2A2A2A), 0);
    lv_obj_set_style_border_width(statsContainer, 0, 0);
    lv_obj_set_style_radius(statsContainer, 8, 0);
    lv_obj_set_style_pad_all(statsContainer, 8, 0);
    lv_obj_clear_flag(statsContainer, LV_OBJ_FLAG_SCROLLABLE);

    // Create stat rows
    _totalDistLabel = _createStatRow(statsContainer, "Total Distance:", "--", 0);
    _totalTimeLabel = _createStatRow(statsContainer, "Total Time:", "--", 30);
    _tripCountLabel = _createStatRow(statsContainer, "Trips:", "--", 60);
    _avgDistLabel = _createStatRow(statsContainer, "Avg Distance:", "--", 90);
    _avgTimeLabel = _createStatRow(statsContainer, "Avg Duration:", "--", 120);
    _avgSpeedLabel = _createStatRow(statsContainer, "Avg Speed:", "--", 150);

    _updateLabels();
}

lv_obj_t* SummaryScreen::_createStatRow(lv_obj_t* parent, const char* label, const char* value, int yPos) {
    // Label
    lv_obj_t* labelObj = lv_label_create(parent);
    lv_label_set_text(labelObj, label);
    lv_obj_set_style_text_font(labelObj, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(labelObj, COLOR_TEXT_SECONDARY, 0);
    lv_obj_set_pos(labelObj, 0, yPos);

    // Value
    lv_obj_t* valueObj = lv_label_create(parent);
    lv_label_set_text(valueObj, value);
    lv_obj_set_style_text_font(valueObj, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(valueObj, COLOR_TEXT_PRIMARY, 0);
    lv_obj_align(valueObj, LV_ALIGN_TOP_RIGHT, 0, yPos);

    return valueObj;
}

void SummaryScreen::_loadStatistics() {
    // Reset statistics
    _stats.totalDistance = 0;
    _stats.totalDuration = 0;
    _stats.tripCount = 0;

    // Try to read from SD card rides directory
    File ridesDir = SD.open("/rides");
    if (!ridesDir || !ridesDir.isDirectory()) {
        // No rides directory - show zeros
        return;
    }

    // Count GPX files and sum up statistics
    // Note: This is a simplified approach - a real implementation would parse GPX files
    File file = ridesDir.openNextFile();
    while (file) {
        if (!file.isDirectory()) {
            String filename = file.name();
            if (filename.endsWith(".gpx")) {
                _stats.tripCount++;
                // In a real implementation, we would parse the GPX file
                // For now, just count the files
            }
        }
        file = ridesDir.openNextFile();
    }
    ridesDir.close();

    // If we have current ride data, add it
    if (_bikeRide != nullptr && _bikeRide->getDistance() > 0) {
        _stats.totalDistance += _bikeRide->getDistance();
        _stats.totalDuration += _bikeRide->getDuration();
    }

    // Calculate averages
    if (_stats.tripCount > 0) {
        _stats.avgDistance = _stats.totalDistance / _stats.tripCount;
        _stats.avgDuration = (double)_stats.totalDuration / _stats.tripCount;
        if (_stats.totalDuration > 0) {
            _stats.avgSpeed = (_stats.totalDistance / ((double)_stats.totalDuration / 3600.0));
        }
    }
}

void SummaryScreen::_updateLabels() {
    char buf[32];

    // Total distance
    if (_settings.imperialUnitsSetting) {
        snprintf(buf, sizeof(buf), "%.1f mi", _stats.totalDistance * 0.621371);
    } else {
        snprintf(buf, sizeof(buf), "%.1f km", _stats.totalDistance);
    }
    lv_label_set_text(_totalDistLabel, buf);

    // Total time
    unsigned long hours = _stats.totalDuration / 3600;
    unsigned long minutes = (_stats.totalDuration % 3600) / 60;
    if (hours > 0) {
        snprintf(buf, sizeof(buf), "%luh %lum", hours, minutes);
    } else {
        snprintf(buf, sizeof(buf), "%lu min", minutes);
    }
    lv_label_set_text(_totalTimeLabel, buf);

    // Trip count
    snprintf(buf, sizeof(buf), "%d", _stats.tripCount);
    lv_label_set_text(_tripCountLabel, buf);

    // Average distance
    if (_settings.imperialUnitsSetting) {
        snprintf(buf, sizeof(buf), "%.1f mi", _stats.avgDistance * 0.621371);
    } else {
        snprintf(buf, sizeof(buf), "%.1f km", _stats.avgDistance);
    }
    lv_label_set_text(_avgDistLabel, buf);

    // Average duration
    unsigned long avgMins = (unsigned long)_stats.avgDuration / 60;
    snprintf(buf, sizeof(buf), "%lu min", avgMins);
    lv_label_set_text(_avgTimeLabel, buf);

    // Average speed
    if (_settings.imperialUnitsSetting) {
        snprintf(buf, sizeof(buf), "%.1f mph", _stats.avgSpeed * 0.621371);
    } else {
        snprintf(buf, sizeof(buf), "%.1f km/h", _stats.avgSpeed);
    }
    lv_label_set_text(_avgSpeedLabel, buf);
}
