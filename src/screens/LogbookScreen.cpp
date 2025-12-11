#include "screens/LogbookScreen.h"
#include <SD.h>

LogbookScreen::LogbookScreen() {
    _detailsMode = false;
    _selectedTripIndex = -1;

    _listContainer = nullptr;
    _tripList = nullptr;
    _detailsContainer = nullptr;
    _detailsTitle = nullptr;
    _startLabel = nullptr;
    _stopLabel = nullptr;
    _durationLabel = nullptr;
    _distanceLabel = nullptr;
    _avgSpeedLabel = nullptr;
    _backButton = nullptr;
    _deleteButton = nullptr;
    _msgbox = nullptr;
}

void LogbookScreen::setSettings(BikeInoSettings settings) {
    _settings = settings;
}

void LogbookScreen::createUI() {
    createBaseLayout();
    _loadTrips();
    _createListMode();
    _createDetailsMode();
    _showListMode();
}

void LogbookScreen::destroyUI() {
    BikeInoScreen::destroyUI();

    _listContainer = nullptr;
    _tripList = nullptr;
    _detailsContainer = nullptr;
    _detailsTitle = nullptr;
    _startLabel = nullptr;
    _stopLabel = nullptr;
    _durationLabel = nullptr;
    _distanceLabel = nullptr;
    _avgSpeedLabel = nullptr;
    _backButton = nullptr;
    _deleteButton = nullptr;
    _msgbox = nullptr;
    _trips.clear();
}

void LogbookScreen::update() {
    // No periodic updates needed
}

void LogbookScreen::_loadTrips() {
    _trips.clear();

    File ridesDir = SD.open("/rides");
    if (!ridesDir || !ridesDir.isDirectory()) {
        return;
    }

    File file = ridesDir.openNextFile();
    while (file) {
        if (!file.isDirectory()) {
            String filename = file.name();
            if (filename.endsWith(".gpx")) {
                TripInfo trip;
                trip.filename = filename;

                // Extract date from filename (assuming format: YYYYMMDD_HHMMSS.gpx)
                if (filename.length() >= 15) {
                    String datePart = filename.substring(0, 8);
                    trip.date = datePart.substring(0, 4) + "-" +
                                datePart.substring(4, 6) + "-" +
                                datePart.substring(6, 8);
                } else {
                    trip.date = "Unknown";
                }

                // Placeholder values - would need to parse GPX for real data
                trip.distance = 0;
                trip.duration = 0;
                trip.avgSpeed = 0;

                _trips.push_back(trip);
            }
        }
        file = ridesDir.openNextFile();
    }
    ridesDir.close();
}

void LogbookScreen::_createListMode() {
    _listContainer = lv_obj_create(_contentPane);
    lv_obj_set_size(_listContainer, CONTENT_PANE_WIDTH - 16, SCREEN_HEIGHT - 16);
    lv_obj_set_pos(_listContainer, 0, 0);
    lv_obj_set_style_bg_color(_listContainer, COLOR_BACKGROUND, 0);
    lv_obj_set_style_border_width(_listContainer, 0, 0);
    lv_obj_set_style_pad_all(_listContainer, 0, 0);
    lv_obj_clear_flag(_listContainer, LV_OBJ_FLAG_SCROLLABLE);

    // Title
    lv_obj_t* title = lv_label_create(_listContainer);
    lv_label_set_text(title, "Logbook");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_18, 0);
    lv_obj_set_style_text_color(title, COLOR_TEXT_PRIMARY, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 0);

    // Trip list
    _tripList = lv_list_create(_listContainer);
    lv_obj_set_size(_tripList, CONTENT_PANE_WIDTH - 16, SCREEN_HEIGHT - 50);
    lv_obj_set_pos(_tripList, 0, 28);
    lv_obj_set_style_bg_color(_tripList, lv_color_hex(0x2A2A2A), 0);
    lv_obj_set_style_border_width(_tripList, 0, 0);
    lv_obj_set_style_radius(_tripList, 8, 0);
    lv_obj_set_style_pad_all(_tripList, 4, 0);

    if (_trips.empty()) {
        lv_obj_t* emptyLabel = lv_label_create(_tripList);
        lv_label_set_text(emptyLabel, "No trips recorded");
        lv_obj_set_style_text_color(emptyLabel, COLOR_TEXT_SECONDARY, 0);
        lv_obj_center(emptyLabel);
    } else {
        for (size_t i = 0; i < _trips.size(); i++) {
            char buf[64];
            snprintf(buf, sizeof(buf), "%s\n%.1f km",
                     _trips[i].date.c_str(),
                     _trips[i].distance);

            lv_obj_t* btn = lv_list_add_button(_tripList, LV_SYMBOL_FILE, buf);
            lv_obj_set_user_data(btn, (void*)(intptr_t)i);
            lv_obj_add_event_cb(btn, _tripSelectedHandler, LV_EVENT_CLICKED, this);
        }
    }
}

void LogbookScreen::_createDetailsMode() {
    _detailsContainer = lv_obj_create(_contentPane);
    lv_obj_set_size(_detailsContainer, CONTENT_PANE_WIDTH - 16, SCREEN_HEIGHT - 16);
    lv_obj_set_pos(_detailsContainer, 0, 0);
    lv_obj_set_style_bg_color(_detailsContainer, COLOR_BACKGROUND, 0);
    lv_obj_set_style_border_width(_detailsContainer, 0, 0);
    lv_obj_set_style_pad_all(_detailsContainer, 0, 0);
    lv_obj_clear_flag(_detailsContainer, LV_OBJ_FLAG_SCROLLABLE);

    // Title
    _detailsTitle = lv_label_create(_detailsContainer);
    lv_label_set_text(_detailsTitle, "Trip Details");
    lv_obj_set_style_text_font(_detailsTitle, &lv_font_montserrat_18, 0);
    lv_obj_set_style_text_color(_detailsTitle, COLOR_TEXT_PRIMARY, 0);
    lv_obj_align(_detailsTitle, LV_ALIGN_TOP_MID, 0, 0);

    // Details container
    lv_obj_t* detailsBox = lv_obj_create(_detailsContainer);
    lv_obj_set_size(detailsBox, CONTENT_PANE_WIDTH - 16, 140);
    lv_obj_set_pos(detailsBox, 0, 28);
    lv_obj_set_style_bg_color(detailsBox, lv_color_hex(0x2A2A2A), 0);
    lv_obj_set_style_border_width(detailsBox, 0, 0);
    lv_obj_set_style_radius(detailsBox, 8, 0);
    lv_obj_set_style_pad_all(detailsBox, 8, 0);
    lv_obj_clear_flag(detailsBox, LV_OBJ_FLAG_SCROLLABLE);

    // Labels
    _startLabel = lv_label_create(detailsBox);
    lv_label_set_text(_startLabel, "Start: --");
    lv_obj_set_style_text_font(_startLabel, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(_startLabel, COLOR_TEXT_PRIMARY, 0);
    lv_obj_set_pos(_startLabel, 0, 0);

    _stopLabel = lv_label_create(detailsBox);
    lv_label_set_text(_stopLabel, "Stop: --");
    lv_obj_set_style_text_font(_stopLabel, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(_stopLabel, COLOR_TEXT_PRIMARY, 0);
    lv_obj_set_pos(_stopLabel, 0, 24);

    _durationLabel = lv_label_create(detailsBox);
    lv_label_set_text(_durationLabel, "Duration: --");
    lv_obj_set_style_text_font(_durationLabel, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(_durationLabel, COLOR_TEXT_PRIMARY, 0);
    lv_obj_set_pos(_durationLabel, 0, 48);

    _distanceLabel = lv_label_create(detailsBox);
    lv_label_set_text(_distanceLabel, "Distance: --");
    lv_obj_set_style_text_font(_distanceLabel, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(_distanceLabel, COLOR_TEXT_PRIMARY, 0);
    lv_obj_set_pos(_distanceLabel, 0, 72);

    _avgSpeedLabel = lv_label_create(detailsBox);
    lv_label_set_text(_avgSpeedLabel, "Avg Speed: --");
    lv_obj_set_style_text_font(_avgSpeedLabel, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(_avgSpeedLabel, COLOR_TEXT_PRIMARY, 0);
    lv_obj_set_pos(_avgSpeedLabel, 0, 96);

    // Buttons container
    lv_obj_t* buttonBox = lv_obj_create(_detailsContainer);
    lv_obj_set_size(buttonBox, CONTENT_PANE_WIDTH - 16, 40);
    lv_obj_set_pos(buttonBox, 0, 176);
    lv_obj_set_style_bg_opa(buttonBox, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(buttonBox, 0, 0);
    lv_obj_set_style_pad_all(buttonBox, 0, 0);
    lv_obj_clear_flag(buttonBox, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_flex_flow(buttonBox, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(buttonBox, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    // Back button
    _backButton = lv_button_create(buttonBox);
    lv_obj_set_size(_backButton, 90, 36);
    lv_obj_set_style_bg_color(_backButton, COLOR_INACTIVE, 0);
    lv_obj_t* backLabel = lv_label_create(_backButton);
    lv_label_set_text(backLabel, LV_SYMBOL_LEFT " Back");
    lv_obj_center(backLabel);
    lv_obj_add_event_cb(_backButton, _backButtonHandler, LV_EVENT_CLICKED, this);

    // Delete button
    _deleteButton = lv_button_create(buttonBox);
    lv_obj_set_size(_deleteButton, 90, 36);
    lv_obj_set_style_bg_color(_deleteButton, lv_color_hex(0xE53935), 0);
    lv_obj_t* deleteLabel = lv_label_create(_deleteButton);
    lv_label_set_text(deleteLabel, LV_SYMBOL_TRASH " Delete");
    lv_obj_center(deleteLabel);
    lv_obj_add_event_cb(_deleteButton, _deleteButtonHandler, LV_EVENT_CLICKED, this);

    // Initially hidden
    lv_obj_add_flag(_detailsContainer, LV_OBJ_FLAG_HIDDEN);
}

void LogbookScreen::_showListMode() {
    _detailsMode = false;
    lv_obj_clear_flag(_listContainer, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(_detailsContainer, LV_OBJ_FLAG_HIDDEN);
}

void LogbookScreen::_showDetailsMode(int tripIndex) {
    if (tripIndex < 0 || tripIndex >= (int)_trips.size()) return;

    _detailsMode = true;
    _selectedTripIndex = tripIndex;

    TripInfo& trip = _trips[tripIndex];

    // Update labels
    char buf[64];

    snprintf(buf, sizeof(buf), "Start: %s", trip.date.c_str());
    lv_label_set_text(_startLabel, buf);

    lv_label_set_text(_stopLabel, "Stop: --");

    unsigned long hours = trip.duration / 3600;
    unsigned long minutes = (trip.duration % 3600) / 60;
    unsigned long seconds = trip.duration % 60;
    snprintf(buf, sizeof(buf), "Duration: %02lu:%02lu:%02lu", hours, minutes, seconds);
    lv_label_set_text(_durationLabel, buf);

    if (_settings.imperialUnitsSetting) {
        snprintf(buf, sizeof(buf), "Distance: %.1f mi", trip.distance * 0.621371);
    } else {
        snprintf(buf, sizeof(buf), "Distance: %.1f km", trip.distance);
    }
    lv_label_set_text(_distanceLabel, buf);

    if (_settings.imperialUnitsSetting) {
        snprintf(buf, sizeof(buf), "Avg Speed: %.1f mph", trip.avgSpeed * 0.621371);
    } else {
        snprintf(buf, sizeof(buf), "Avg Speed: %.1f km/h", trip.avgSpeed);
    }
    lv_label_set_text(_avgSpeedLabel, buf);

    lv_obj_add_flag(_listContainer, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(_detailsContainer, LV_OBJ_FLAG_HIDDEN);
}

void LogbookScreen::_showDeleteConfirmation() {
    static const char* btns[] = {"Yes", "No", ""};

    _msgbox = lv_msgbox_create(_screen);
    lv_msgbox_add_title(_msgbox, "Confirm Delete");
    lv_msgbox_add_text(_msgbox, "Delete this trip?");
    lv_msgbox_add_close_button(_msgbox);

    lv_obj_t* btnYes = lv_msgbox_add_footer_button(_msgbox, "Yes");
    lv_obj_t* btnNo = lv_msgbox_add_footer_button(_msgbox, "No");

    lv_obj_add_event_cb(btnYes, _msgboxHandler, LV_EVENT_CLICKED, this);
    lv_obj_add_event_cb(btnNo, _msgboxHandler, LV_EVENT_CLICKED, this);

    lv_obj_center(_msgbox);
}

void LogbookScreen::_deleteSelectedTrip() {
    if (_selectedTripIndex < 0 || _selectedTripIndex >= (int)_trips.size()) return;

    String filepath = "/rides/" + _trips[_selectedTripIndex].filename;
    SD.remove(filepath.c_str());

    // Reload trips
    _loadTrips();

    // Recreate list
    if (_tripList != nullptr) {
        lv_obj_delete(_tripList);
        _tripList = nullptr;
    }

    // Recreate the list
    _tripList = lv_list_create(_listContainer);
    lv_obj_set_size(_tripList, CONTENT_PANE_WIDTH - 16, SCREEN_HEIGHT - 50);
    lv_obj_set_pos(_tripList, 0, 28);
    lv_obj_set_style_bg_color(_tripList, lv_color_hex(0x2A2A2A), 0);
    lv_obj_set_style_border_width(_tripList, 0, 0);
    lv_obj_set_style_radius(_tripList, 8, 0);

    if (_trips.empty()) {
        lv_obj_t* emptyLabel = lv_label_create(_tripList);
        lv_label_set_text(emptyLabel, "No trips recorded");
        lv_obj_set_style_text_color(emptyLabel, COLOR_TEXT_SECONDARY, 0);
        lv_obj_center(emptyLabel);
    } else {
        for (size_t i = 0; i < _trips.size(); i++) {
            char buf[64];
            snprintf(buf, sizeof(buf), "%s\n%.1f km",
                     _trips[i].date.c_str(),
                     _trips[i].distance);

            lv_obj_t* btn = lv_list_add_button(_tripList, LV_SYMBOL_FILE, buf);
            lv_obj_set_user_data(btn, (void*)(intptr_t)i);
            lv_obj_add_event_cb(btn, _tripSelectedHandler, LV_EVENT_CLICKED, this);
        }
    }

    _showListMode();
}

// Event handlers
void LogbookScreen::_tripSelectedHandler(lv_event_t* e) {
    lv_obj_t* target = static_cast<lv_obj_t*>(lv_event_get_target(e));
    LogbookScreen* screen = static_cast<LogbookScreen*>(lv_event_get_user_data(e));

    int index = (int)(intptr_t)lv_obj_get_user_data(target);
    if (screen != nullptr) {
        screen->_showDetailsMode(index);
    }
}

void LogbookScreen::_backButtonHandler(lv_event_t* e) {
    LogbookScreen* screen = static_cast<LogbookScreen*>(lv_event_get_user_data(e));
    if (screen != nullptr) {
        screen->_showListMode();
    }
}

void LogbookScreen::_deleteButtonHandler(lv_event_t* e) {
    LogbookScreen* screen = static_cast<LogbookScreen*>(lv_event_get_user_data(e));
    if (screen != nullptr) {
        screen->_showDeleteConfirmation();
    }
}

void LogbookScreen::_msgboxHandler(lv_event_t* e) {
    lv_obj_t* target = static_cast<lv_obj_t*>(lv_event_get_target(e));
    LogbookScreen* screen = static_cast<LogbookScreen*>(lv_event_get_user_data(e));

    const char* btnText = lv_label_get_text(lv_obj_get_child(target, 0));

    if (screen != nullptr && screen->_msgbox != nullptr) {
        if (strcmp(btnText, "Yes") == 0) {
            screen->_deleteSelectedTrip();
        }
        lv_msgbox_close(screen->_msgbox);
        screen->_msgbox = nullptr;
    }
}
