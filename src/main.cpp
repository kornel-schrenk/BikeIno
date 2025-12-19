#include <M5Unified.h>
#include <WiFi.h>

#define LV_CONF_INCLUDE_SIMPLE
#include <lvgl.h>
#include <esp_timer.h>
#include <TinyGPS++.h>

#include "ui/UIConstants.h"
#include "ui/NavigationPane.h"
#include "streamback/StreamBackClient.h"

#include "screens/BikeInoScreen.h"
#include "screens/RideScreen.h"
#include "screens/LogbookScreen.h"
#include "screens/SensorScreen.h"
#include "screens/SummaryScreen.h"
#include "screens/SettingsScreen.h"

#include "utils/SettingsUtils.h"

#include "ride/BikeRide.h"

#define HALL_SENSOR_PIN 36

const String VERSION_NUMBER = "0.2.2";

///////////////
// Bike Ride //
///////////////

BikeRide bikeRide = BikeRide();

/////////////
// Sensors //
/////////////

const unsigned long GPSBaudRate = 9600;
const int GPSUartNumber = 2;

// The serial connection to the GPS device
HardwareSerial hardwareSerial(GPSUartNumber);
// The TinyGPS++ object
TinyGPSPlus gps;

// Hall sensor state
volatile unsigned long lastHallPulseTime = 0;
volatile unsigned long hallPulseInterval = 0;
volatile bool hallSensorActive = false;

///////////
// Utils //
///////////

SettingsUtils settingsUtils = SettingsUtils();

////////////////
// StreamBack //
////////////////

// Hardcoded configuration for development
const char* WIFI_SSID = "YOUR_WIFI_SSID";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";
const char* MQTT_BROKER = "streamback.example.com";
const int MQTT_PORT = 1883;
const char* MQTT_TOPIC = "streamback/bikeino/data";

StreamBackClient streamBackClient;

// Auto-ride management
bool streamBackAutoRideStarted = false;
unsigned long lastStreamBackDataTime = 0;

/////////////
// SCREENS //
/////////////

int _currentScreenId = SCREEN_RIDE;
BikeInoScreen* _activeScreen = nullptr;

RideScreen rideScreen = RideScreen(&bikeRide);
LogbookScreen logbookScreen = LogbookScreen();
SensorScreen sensorScreen = SensorScreen();
SummaryScreen summaryScreen = SummaryScreen(&bikeRide);
SettingsScreen settingsScreen = SettingsScreen(&settingsUtils);

//////////
// LVGL //
//////////

lv_display_t* lvDisplay;
lv_indev_t* lvIndev;

// Navigation pane (shared across all screens)
NavigationPane navigationPane;

// Battery update timer
unsigned long lastBatteryUpdate = 0;
const unsigned long BATTERY_UPDATE_INTERVAL = 30000; // 30 seconds

/////////////////////
// Utility methods //
/////////////////////

void lvglFlushDisplay(lv_display_t* display, const lv_area_t* area, uint8_t* px_map) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    lv_draw_sw_rgb565_swap(px_map, w * h);
    M5.Display.pushImageDMA<uint16_t>(area->x1, area->y1, w, h, (uint16_t*)px_map);
    lv_disp_flush_ready(display);
}

uint32_t lvglTickFunction() {
    return (esp_timer_get_time() / 1000LL);
}

void touchpadHandler(lv_indev_t* drv, lv_indev_data_t* data) {
    M5.update();
    auto count = M5.Touch.getCount();

    if (count == 0) {
        data->state = LV_INDEV_STATE_RELEASED;
    } else {
        auto touch = M5.Touch.getDetail(0);
        data->state = LV_INDEV_STATE_PRESSED;
        data->point.x = touch.x;
        data->point.y = touch.y;
    }
}

// Hall sensor interrupt handler
void IRAM_ATTR hallSensorISR() {
    unsigned long currentTime = micros();
    if (currentTime - lastHallPulseTime > 50000) { // Debounce: ignore pulses within 50ms
        hallPulseInterval = currentTime - lastHallPulseTime;
        lastHallPulseTime = currentTime;
        hallSensorActive = true;
    }
}

///////////////////////
// Screen Management //
///////////////////////

void switchToScreen(int screenId);

void navigationCallback(int screenId) {
    if (screenId != _currentScreenId) {
        switchToScreen(screenId);
    }
}

BikeInoScreen* getScreenById(int screenId) {
    switch (screenId) {
        case SCREEN_RIDE:
            return &rideScreen;
        case SCREEN_SENSOR:
            return &sensorScreen;
        case SCREEN_LOGBOOK:
            return &logbookScreen;
        case SCREEN_SUMMARY:
            return &summaryScreen;
        case SCREEN_SETTINGS:
            return &settingsScreen;
        default:
            return &rideScreen;
    }
}

void switchToScreen(int screenId) {
    Serial.print(F("Switching to screen: "));
    Serial.println(screenId);

    // Destroy current screen UI if exists
    if (_activeScreen != nullptr && _activeScreen->isCreated()) {
        _activeScreen->destroyUI();
    }

    // Get the new screen
    _activeScreen = getScreenById(screenId);
    _currentScreenId = screenId;

    // Set up screen-specific data
    BikeInoSettings currentSettings = settingsUtils.getBikeInoSettings();
    if (screenId == SCREEN_RIDE) {
        rideScreen.setGps(&gps);
        rideScreen.setSettings(currentSettings);
    } else if (screenId == SCREEN_SENSOR) {
        sensorScreen.setGps(&gps);
        sensorScreen.setSettings(currentSettings);
    } else if (screenId == SCREEN_LOGBOOK) {
        logbookScreen.setSettings(currentSettings);
    } else if (screenId == SCREEN_SUMMARY) {
        summaryScreen.setSettings(currentSettings);
    } else if (screenId == SCREEN_SETTINGS) {
        settingsScreen.setSettings(currentSettings);
    }

    // Create the new screen UI
    _activeScreen->createUI();

    // Create navigation pane on the new screen
    navigationPane.create(_activeScreen->getScreen());
    navigationPane.setActiveScreen(screenId);
    navigationPane.setNavigationCallback(navigationCallback);

    // Update battery indicator
    int batteryLevel = M5.Power.getBatteryLevel();
    navigationPane.updateBattery(batteryLevel);

    // Load the screen
    lv_screen_load(_activeScreen->getScreen());
}

void updateBatteryIndicator() {
    unsigned long currentTime = millis();
    if (currentTime - lastBatteryUpdate >= BATTERY_UPDATE_INTERVAL) {
        lastBatteryUpdate = currentTime;
        int batteryLevel = M5.Power.getBatteryLevel();
        navigationPane.updateBattery(batteryLevel);
    }
}

///////////////////////
// Lifecycle methods //
///////////////////////

void setup() {
    M5.begin();

    Serial.begin(115200);

    hardwareSerial.begin(GPSBaudRate);

    Serial.println("\n");
    Serial.println(F("***********"));
    Serial.println(F("* BikeIno *"));
    Serial.println(F("***********"));

    Serial.print(F("v"));
    Serial.print(VERSION_NUMBER);
    Serial.println("\n");

    if (!SD.begin()) {
        Serial.println(F("SD Card Mount Failed"));
    }

    Serial.println(F("LVGL init..."));
    lv_init();
    lv_tick_set_cb(lvglTickFunction);

    lvDisplay = lv_display_create(SCREEN_WIDTH, SCREEN_HEIGHT);
    lv_display_set_flush_cb(lvDisplay, lvglFlushDisplay);

    static lv_color_t displayBuffer[SCREEN_WIDTH * 15];
    lv_display_set_buffers(lvDisplay, displayBuffer, nullptr, sizeof(displayBuffer), LV_DISPLAY_RENDER_MODE_PARTIAL);

    lvIndev = lv_indev_create();
    lv_indev_set_type(lvIndev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(lvIndev, touchpadHandler);

    Serial.println(F("Hall sensor init..."));
    pinMode(HALL_SENSOR_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(HALL_SENSOR_PIN), hallSensorISR, FALLING);

    Serial.println(F("Power init..."));
    M5.Power.begin();

    Serial.println(F("StreamBack init..."));
    streamBackClient.begin(WIFI_SSID, WIFI_PASSWORD, MQTT_BROKER, MQTT_PORT, MQTT_TOPIC);

    Serial.println(F("Loading initial screen..."));
    switchToScreen(SCREEN_RIDE);

    Serial.println(F("BikeIno init completed!"));
}

void loop() {
    // Feed GPS data
    while (hardwareSerial.available() > 0) {
        gps.encode(hardwareSerial.read());
    }

    // Check if Hall sensor has timed out (no pulse for 3 seconds = stopped)
    if (hallSensorActive && (micros() - lastHallPulseTime > 3000000)) {
        hallSensorActive = false;
        hallPulseInterval = 0;
    }

    // StreamBack MQTT processing
    streamBackClient.loop();

    // Get StreamBack data
    StreamBackData sbData = streamBackClient.getData();

    // Auto-start ride when StreamBack data arrives
    if (sbData.isValid && !streamBackAutoRideStarted && !bikeRide.isRideInProgress()) {
        Serial.println(F("StreamBack: Auto-starting ride"));
        bikeRide.startRide(sbData.speed, sbData.latitude, sbData.longitude, sbData.altitude);
        streamBackAutoRideStarted = true;
        lastStreamBackDataTime = millis();
    }

    // Auto-stop ride when StreamBack data times out (5 seconds)
    if (streamBackAutoRideStarted && !sbData.isValid) {
        if (millis() - lastStreamBackDataTime > 5000) {
            Serial.println(F("StreamBack: Auto-stopping ride (timeout)"));
            bikeRide.stopRide();
            streamBackAutoRideStarted = false;
        }
    }

    // Update last data time when valid
    if (sbData.isValid) {
        lastStreamBackDataTime = millis();
    }

    // Progress ride with StreamBack data if active
    if (bikeRide.isRideInProgress() && sbData.isValid && !bikeRide.isRidePaused()) {
        // Convert StreamBack duration to UTC time string (HH:MM:SS format)
        unsigned long durationSec = sbData.duration / 1000;
        char utcTime[12];
        snprintf(utcTime, sizeof(utcTime), "%02lu:%02lu:%02lu",
                 (durationSec / 3600) % 24, (durationSec / 60) % 60, durationSec % 60);

        bikeRide.progressRide(true, sbData.speed, sbData.latitude,
                              sbData.longitude, sbData.altitude, String(utcTime));
    }

    // Update sensor data for RideScreen
    if (_currentScreenId == SCREEN_RIDE) {
        rideScreen.setHallSensorData(hallSensorActive, hallPulseInterval);
        rideScreen.setStreamBackData(sbData);
    }

    // Update the active screen
    if (_activeScreen != nullptr) {
        _activeScreen->update();
    }

    // Periodically update battery indicator
    updateBatteryIndicator();

    // LVGL task handler
    lv_task_handler();
    vTaskDelay(1);
}
