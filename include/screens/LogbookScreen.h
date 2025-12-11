#ifndef LogbookScreen_h
#define LogbookScreen_h

#include "Arduino.h"
#include <vector>

#include "screens/BikeInoScreen.h"

// Trip info structure
struct TripInfo {
    String filename;
    String date;
    double distance;
    unsigned long duration;
    double avgSpeed;
};

class LogbookScreen : public BikeInoScreen {

public:
    LogbookScreen();

    // Override base class methods
    void createUI() override;
    void destroyUI() override;
    void update() override;

    // Set settings
    void setSettings(BikeInoSettings settings);

private:
    bool _detailsMode;
    int _selectedTripIndex;
    std::vector<TripInfo> _trips;

    // List mode widgets
    lv_obj_t* _listContainer;
    lv_obj_t* _tripList;

    // Details mode widgets
    lv_obj_t* _detailsContainer;
    lv_obj_t* _detailsTitle;
    lv_obj_t* _startLabel;
    lv_obj_t* _stopLabel;
    lv_obj_t* _durationLabel;
    lv_obj_t* _distanceLabel;
    lv_obj_t* _avgSpeedLabel;
    lv_obj_t* _backButton;
    lv_obj_t* _deleteButton;

    // Message box for confirmation
    lv_obj_t* _msgbox;

    void _createListMode();
    void _createDetailsMode();
    void _loadTrips();
    void _showListMode();
    void _showDetailsMode(int tripIndex);
    void _showDeleteConfirmation();
    void _deleteSelectedTrip();

    // Event handlers
    static void _tripSelectedHandler(lv_event_t* e);
    static void _backButtonHandler(lv_event_t* e);
    static void _deleteButtonHandler(lv_event_t* e);
    static void _msgboxHandler(lv_event_t* e);
};

#endif
