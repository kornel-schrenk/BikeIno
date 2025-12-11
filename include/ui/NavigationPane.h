#ifndef NavigationPane_h
#define NavigationPane_h

#include <lvgl.h>
#include "screens/BikeInoScreen.h"
#include "ui/UIConstants.h"

// Callback type for screen navigation
typedef void (*NavigationCallback)(int screenId);

class NavigationPane {
public:
    NavigationPane();

    // Create the navigation pane on the given parent object
    void create(lv_obj_t* parent);

    // Destroy the navigation pane
    void destroy();

    // Set the active screen (highlights the corresponding icon)
    void setActiveScreen(int screenId);

    // Update the battery indicator
    void updateBattery(int percent);

    // Set the callback for navigation events
    void setNavigationCallback(NavigationCallback callback);

    // Get the navigation pane object
    lv_obj_t* getPane();

private:
    lv_obj_t* _pane;
    lv_obj_t* _batteryIcon;
    lv_obj_t* _batteryLabel;
    lv_obj_t* _screenIcons[5];  // Ride, Sensor, Logbook, Summary, Settings

    NavigationCallback _navCallback;
    int _activeScreenId;

    void _createBatteryIndicator();
    void _createScreenIcons();
    void _updateIconStyles();

    static void _iconClickHandler(lv_event_t* e);
};

#endif
