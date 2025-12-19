#ifndef RideScreen_h
#define RideScreen_h

#include "Arduino.h"
#include <TinyGPS++.h>

#include "ride/BikeRide.h"
#include "screens/BikeInoScreen.h"

class RideScreen : public BikeInoScreen {

public:
    RideScreen(BikeRide* bikeRide);

    // Override base class methods
    void createUI() override;
    void destroyUI() override;
    void update() override;

    // Set GPS reference for updates
    void setGps(TinyGPSPlus* gps);

    // Set Hall sensor data
    void setHallSensorData(bool active, unsigned long pulseInterval);

    // Set StreamBack data
    void setStreamBackData(const StreamBackData& data);

    // Set settings
    void setSettings(BikeInoSettings settings);

private:
    BikeRide* _bikeRide;
    TinyGPSPlus* _gps;

    // Sensor states
    bool _gpsEnabled;
    bool _hallEnabled;
    bool _hallActive;
    unsigned long _hallPulseInterval;

    // StreamBack state
    bool _streamBackEnabled;
    StreamBackData _streamBackData;

    // Top pane - sensor indicators
    lv_obj_t* _topPane;
    lv_obj_t* _streamBackButton;
    lv_obj_t* _streamBackIcon;
    lv_obj_t* _gpsButton;
    lv_obj_t* _gpsIcon;
    lv_obj_t* _hallButton;
    lv_obj_t* _hallIcon;

    // Middle pane - speed display
    lv_obj_t* _middlePane;
    lv_obj_t* _speedLabel;
    lv_obj_t* _speedUnitLabel;

    // Bottom pane - info tiles
    lv_obj_t* _bottomPane;
    lv_obj_t* _timeLabel;
    lv_obj_t* _temperatureLabel;
    lv_obj_t* _totalDistanceLabel;
    lv_obj_t* _tripDurationLabel;
    lv_obj_t* _tripDistanceLabel;

    // Ride control buttons
    lv_obj_t* _startButton;
    lv_obj_t* _pauseButton;
    lv_obj_t* _stopButton;

    // UI creation helpers
    void _createTopPane();
    void _createMiddlePane();
    void _createBottomPane();

    // Update helpers
    void _updateSensorIcons();
    void _updateSpeedDisplay();
    void _updateInfoTiles();
    void _updateRideControls();

    // Calculate speed from Hall sensor
    double _calculateSpeedFromHall();

    // Event handlers
    static void _streamBackButtonHandler(lv_event_t* e);
    static void _gpsButtonHandler(lv_event_t* e);
    static void _hallButtonHandler(lv_event_t* e);
    static void _startButtonHandler(lv_event_t* e);
    static void _pauseButtonHandler(lv_event_t* e);
    static void _stopButtonHandler(lv_event_t* e);
};

#endif
