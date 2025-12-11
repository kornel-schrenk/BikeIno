#ifndef SensorScreen_h
#define SensorScreen_h

#include "Arduino.h"
#include <TinyGPS++.h>

#include "screens/BikeInoScreen.h"

class SensorScreen : public BikeInoScreen {

public:
    SensorScreen();

    // Override base class methods
    void createUI() override;
    void destroyUI() override;
    void update() override;

    // Set GPS reference for updates
    void setGps(TinyGPSPlus* gps);

    // Set Hall sensor data
    void setHallSensorData(bool active, unsigned long pulseInterval, unsigned int rpm);

    // Set settings
    void setSettings(BikeInoSettings settings);

private:
    TinyGPSPlus* _gps;

    // Hall sensor state
    bool _hallActive;
    unsigned long _hallPulseInterval;
    unsigned int _hallRpm;

    // Hall sensor pane
    lv_obj_t* _hallPane;
    lv_obj_t* _hallIcon;
    lv_obj_t* _hallRpmLabel;

    // GPS sensor pane
    lv_obj_t* _gpsPane;
    lv_obj_t* _gpsIcon;
    lv_obj_t* _gpsSpeedLabel;
    lv_obj_t* _gpsLatLabel;
    lv_obj_t* _gpsLngLabel;
    lv_obj_t* _gpsEleLabel;

    void _createHallPane();
    void _createGpsPane();
    void _updateHallPane();
    void _updateGpsPane();
};

#endif
