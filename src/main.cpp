#include <M5Unified.h>

#define LV_CONF_INCLUDE_SIMPLE
#include <lvgl.h>
#include <esp_timer.h>
#include <TinyGPS++.h>

#include "screens/BikeInoScreen.h"
#include "screens/RideScreen.h"
#include "screens/LogbookScreen.h"
#include "screens/SensorScreen.h"
#include "screens/SummaryScreen.h"

#include "utils/SettingsUtils.h"

#include "ride/BikeRide.h"

#define HALL (36)

const String VERSION_NUMBER = "0.1.1";

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

///////////
// Utils //
///////////

SettingsUtils settingsUtils = SettingsUtils();

/////////////
// SCREENS //
/////////////

// Screen properties
int _currentScreen = SCREEN_RIDE;
int16_t _lastPickedMainMenuIndex = 1;
bool _backToMenu = false;

RideScreen rideScreen = RideScreen(&bikeRide);
LogbookScreen logbookScreen = LogbookScreen();
SensorScreen sensorScreen = SensorScreen();
SummaryScreen summaryScreen = SummaryScreen(&bikeRide);

//////////
// LVGL //
//////////

constexpr int32_t DISPLAY_RES_X = 320;
constexpr int32_t DISPLAY_RES_Y = 240;

lv_display_t *display;
lv_indev_t *indev;

/////////////////////
// Utility methods //
/////////////////////

void lvglFlushDisplay(lv_display_t *display, const lv_area_t *area, uint8_t *px_map) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  lv_draw_sw_rgb565_swap(px_map, w*h);
  M5.Display.pushImageDMA<uint16_t>(area->x1, area->y1, w, h, (uint16_t *)px_map);
  lv_disp_flush_ready(display);
}

uint32_t lvglTickFunction() {
  return (esp_timer_get_time() / 1000LL);
}

void openRideScreen()
{
  _backToMenu = false;
  _currentScreen = SCREEN_RIDE;
  rideScreen.init(settingsUtils.getBikeInoSettings(), gps);
}

void openSensorScreen()
{
  _backToMenu = false;
  _currentScreen = SCREEN_SENSOR;
  sensorScreen.init(settingsUtils.getBikeInoSettings());
}

void openSummaryScreen()
{
  _backToMenu = false;
  _currentScreen = SCREEN_SUMMARY;
  summaryScreen.init(settingsUtils.getBikeInoSettings());
}

void touchpadHandler(lv_indev_t * drv, lv_indev_data_t * data) {
  M5.update();
  auto count = M5.Touch.getCount();

  if ( count == 0 ) {
    data->state = LV_INDEV_STATE_RELEASED;
  } else {
    auto touch = M5.Touch.getDetail(0);
    data->state = LV_INDEV_STATE_PRESSED; 
    data->point.x = touch.x;
    data->point.y = touch.y;
  }
}

void buttonClickHandler(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *label = reinterpret_cast<lv_obj_t *>(lv_event_get_user_data(e));

  switch (code)
  {
  case LV_EVENT_PRESSED:
    lv_label_set_text(label, "The last button event:\nLV_EVENT_PRESSED");
    break;
  case LV_EVENT_CLICKED:
    lv_label_set_text(label, "The last button event:\nLV_EVENT_CLICKED");
    break;
  case LV_EVENT_LONG_PRESSED:
    lv_label_set_text(label, "The last button event:\nLV_EVENT_LONG_PRESSED");
    break;
  case LV_EVENT_LONG_PRESSED_REPEAT:
    lv_label_set_text(label, "The last button event:\nLV_EVENT_LONG_PRESSED_REPEAT");
    break;
  default:
    break;
  }
}

void initDisplay() {
  lv_obj_t *btn = lv_button_create(lv_screen_active());
  lv_obj_set_size(btn, 100, 50);
  lv_obj_center(btn);

  lv_obj_t *btn_label = lv_label_create(btn);
  lv_label_set_text(btn_label, "Click me!");
  lv_obj_center(btn_label);

  lv_obj_t *info_label = lv_label_create(lv_screen_active());
  lv_label_set_text(info_label, "The last button event:\nNone");

  lv_obj_add_event_cb(btn, buttonClickHandler, LV_EVENT_ALL, info_label);  
}

///////////////////////
// Lifecycle methods //
///////////////////////

void setup()
{
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

  display = lv_display_create(DISPLAY_RES_X, DISPLAY_RES_Y);
  lv_display_set_flush_cb(display, lvglFlushDisplay);

  static lv_color_t displayBuffer[DISPLAY_RES_X * 15]; 
  lv_display_set_buffers(display, displayBuffer, nullptr, sizeof(displayBuffer), LV_DISPLAY_RENDER_MODE_PARTIAL);

  indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
  lv_indev_set_read_cb(indev, touchpadHandler);

  Serial.println(F("Display init... "));
  initDisplay();

  Serial.println(F("Hall sensor init... "));
  M5.Power.begin(); 
  pinMode(HALL, INPUT); 
  
  Serial.println(F("BikeIno init completed!"));  
}

void loop()
{
  lv_task_handler();
  vTaskDelay(1);

  bool hallStatus = digitalRead(HALL);
  Serial.print(F("Hall sensor status: "));
  Serial.println(hallStatus);  
}