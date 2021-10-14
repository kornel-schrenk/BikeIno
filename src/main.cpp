#include <M5ez.h>
#include <M5Stack.h>
#include <TinyGPS++.h>

#include "images/jpgs.h"
#include "images/jpgsdark.h"

#include "screens/MainMenu.h"
#include "screens/BikeInoScreen.h"
#include "screens/HomeScreen.h"
#include "screens/RideScreen.h"
#include "screens/SpeedScreen.h"
#include "screens/LogbookScreen.h"
#include "screens/SensorScreen.h"

#include "pickers/SettingsPicker.h"

#include "utils/TimeUtils.h"
#include "utils/SettingsUtils.h"

#include "ride/BikeRide.h"

const String VERSION_NUMBER = "0.0.5";

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

TimeUtils timeUtils = TimeUtils();
SettingsUtils settingsUtils = SettingsUtils();

/////////////
// SCREENS //
/////////////

// Screen properties
int _currentScreen = SCREEN_HOME;
int16_t _lastPickedMainMenuIndex = 1;
bool _backToMenu = false;

MainMenu mainMenuScreen = MainMenu();
HomeScreen homeScreen = HomeScreen(timeUtils);
RideScreen rideScreen = RideScreen(&bikeRide);
SpeedScreen speedScreen = SpeedScreen();
LogbookScreen logbookScreen = LogbookScreen();
SensorScreen sensorScreen = SensorScreen();

SettingsPicker settingsPicker;

/////////////////////
// Utility methods //
/////////////////////

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

///////////////////////
// Lifecycle methods //
///////////////////////

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  M5.Power.begin();

  hardwareSerial.begin(GPSBaudRate);

#include <themes/default.h>
#include <themes/dark.h>
  ez.begin();

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

  homeScreen.initHomeScreen(settingsUtils.getBikeInoSettings());
}

void loop()
{
  // In every second load sensor data
  if (secondChanged())
  {
    while (hardwareSerial.available() > 0)
    {
      gps.encode(hardwareSerial.read());
    }

    if (gps.charsProcessed() < 10)
    {
      Serial.println(F("WARNING: No GPS data.  Check wiring."));
    }
  }

  String buttonPressed = "";
  if (!_backToMenu)
  {
    buttonPressed = ez.buttons.poll();
    if (buttonPressed != "")
    {
      Serial.println("Button pressed: " + buttonPressed);
    }
  }
  if (_backToMenu || buttonPressed == "Menu")
  {
    ezMenu mainMenu = mainMenuScreen.initMainMenu();
    // Set the menu selection based on the last visited menu item
    mainMenu.pickItem(_lastPickedMainMenuIndex - 1);
    // Run the stuff behind the menu item and return with its index + 1
    _lastPickedMainMenuIndex = mainMenu.runOnce();

    switch (_lastPickedMainMenuIndex)
    {
    case 0:
      _backToMenu = false;
      _currentScreen = SCREEN_HOME;
      homeScreen.initHomeScreen(settingsUtils.getBikeInoSettings());
      break;
    case 1:
      openRideScreen();
      break;
    case 2:
      _backToMenu = false;
      _currentScreen = SCREEN_SPEED;
      speedScreen.init(settingsUtils.getBikeInoSettings());
      break;
    case 3:
      _backToMenu = false;
      _currentScreen = SCREEN_LOGBOOK;
      logbookScreen.init(settingsUtils.getBikeInoSettings());
      break;
    case 4:
      openSensorScreen();
      break;
    case 5:
      settingsPicker.runOnce("Settings");
      _backToMenu = true;
      _currentScreen = SCREEN_SETTINGS;
      break;
    case 6:
      _currentScreen = SCREEN_ABOUT;
      _backToMenu = true;
      ez.msgBox("About", "BikeIno | Version: " + VERSION_NUMBER + "| Author: kornel@schrenk.hu", "Menu");
      break;
    }
  }
  else if (buttonPressed != "")
  {
    //Handle button press on the current screen
    switch (_currentScreen)
    {
    case SCREEN_HOME:
      if (homeScreen.handleButtonPress(buttonPressed) == SCREEN_RIDE)
      {
        openRideScreen();
      }
      break;
    case SCREEN_RIDE:
      rideScreen.handleButtonPress(buttonPressed);
      break;
    case SCREEN_SPEED:
      speedScreen.handleButtonPress(buttonPressed);
      break;
    case SCREEN_LOGBOOK:
      logbookScreen.handleButtonPress(buttonPressed);
      break;
    case SCREEN_SENSOR:
      sensorScreen.handleButtonPress(buttonPressed);
      break;
    }
  }
  else
  {
    //NO Button was pressed - Normal operation
    switch (_currentScreen)
    {
    case SCREEN_HOME:
      homeScreen.displayHomeClock();
      break;
    case SCREEN_RIDE:
      if (minuteChanged())
      {
        rideScreen.refreshClockWidget();
      }
      if (minuteChanged() || secondChanged())
      {
        rideScreen.display(gps);        
      }
      break;
    case SCREEN_SPEED:
      if (minuteChanged())
      {
        speedScreen.refreshClockWidget();
      }
      if (minuteChanged() || secondChanged())
      {
        speedScreen.display(gps);
      }
      break;
    case SCREEN_LOGBOOK:
      if (minuteChanged())
      {
        logbookScreen.refreshClockWidget();
      }
      break;
    case SCREEN_SENSOR:
      if (minuteChanged())
      {
        sensorScreen.refreshClockWidget();
      }
      if (minuteChanged() || secondChanged())
      {
        sensorScreen.display(gps);
      }
      break;
    case SCREEN_SETTINGS:
      if (minuteChanged())
      {
        settingsPicker.refreshClockWidget();
      }
      break;
    }
  }
}