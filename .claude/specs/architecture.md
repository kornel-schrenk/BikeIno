# Hardware

The Hardware inventory of the BikeIno project consist of the following parts:
1. [M5Stack Core 2](https://shop.m5stack.com/products/m5stack-core2-esp32-iot-development-kit?variant=35960244109476) based on ESP32 as the Controller Board
2. GPS units can be one of:
    1. [GPS/BDS Unit v1.1](https://shop.m5stack.com/products/gps-bds-unit-v1-1-at6668)
    2. [GPS Module v2.1](https://shop.m5stack.com/products/gps-module-v2-1-with-antenna-atgm336h)
3. [Hall sensor Unit](https://shop.m5stack.com/products/hall-effect-unit-a3144e)
4. [Groove Cable](https://shop.m5stack.com/products/4pin-buckled-grove-cable?variant=32050974621786) - 1 meter

## Hardware Abstraction
- Uses **M5Unified** library for M5Stack hardware abstraction (display, touch, SD card, power)
- GPS data via **TinyGPS++** library over HardwareSerial (UART2, 9600 baud)
- Hall sensor on GPIO 36 for speed measurement
- Display resolution: 320x240

# Software

The BikeIno software is developed in Visual Studio code with the [PlaformIO](https://platformio.org/) plug-in.

The following libraries are used:
1. [M5Unified](https://github.com/m5stack/M5Unified) for the M5Stack Core 2 specific features
2. [TinyGPSPlus](https://github.com/mikalhart/TinyGPSPlus) for GPS integration
3. [LVGL](https://github.com/lvgl/lvgl/releases/tag/v9.3.0) for the UI development

## Frontend

BikeIno frontend is created with the LVGL 9.3 library. The idea is that in the main.cpp file the LVGL display gets created.

- LVGL configuration in [include/lv_conf.h](include/lv_conf.h)
- Display buffer uses partial rendering mode with DMA push

Each screen has a separate class inherited from the *BikeInoScreen*, which provides common functionality. For instance the *RideScreen* has an associated LVGL screen, which gets created in the *RideScreen* class. When the User navigates through the screens the right screen gets displayed.

### Screen System
- Base class: `BikeInoScreen` in [include/screens/BikeInoScreen.h](include/screens/BikeInoScreen.h)
- Screen constants: `SCREEN_RIDE`, `SCREEN_SENSOR`, `SCREEN_LOGBOOK`, `SCREEN_SUMMARY`, `SCREEN_SETTINGS`
- `STAY_ON_SCREEN` (456) returned by handlers to remain on current screen
- Settings passed via `BikeInoSettings` struct to screen `init()` methods
- GPS object (`TinyGPSPlus`) passed to screens for location data
- Screen handlers return screen constants for navigation or `STAY_ON_SCREEN`

## Backend

The *BikeRide* class manages the actual bicycle ride. It gets called in the main loop. The *RideScreen* can manage the state of the ride:
1. It can start
2. It can pause
3. It can stop

Once a new bicycle ride was started a new GPX file gets created. Basically it is a new **Trip**. These Trips will be displayed in the LogBook screen. During the ride its data gets displayed on the *RideScreen*. The Settings influence the presentation. For instance the current speed can be displayed in metric or imperial units.

### Core Classes

**BikeRide** ([include/ride/BikeRide.h](include/ride/BikeRide.h))
- Manages ride state (in progress, paused)
- Tracks RideData: lat/lng, speed, altitude, distance, duration
- Lifecycle: `startRide()` → `progressRide()` → `pauseRide()`/`restartRide()` → `stopRide()`

**GPX** ([include/utils/GPX.h](include/utils/GPX.h))
- Generates GPX 1.1 XML format for ride logging
- Supports track points with elevation and timestamp

**FileUtils** ([include/utils/FileUtils.h](include/utils/FileUtils.h))
- SD card file operations for ride logs

**SettingsUtils** ([include/utils/SettingsUtils.h](include/utils/SettingsUtils.h))
- Manages BikeInoSettings (sound, imperial units)

## Directory Structure
```
src/
  main.cpp           # Entry point, LVGL setup, main loop
  screens/           # Screen implementations
  ride/              # BikeRide logic
  utils/             # GPX, file, settings utilities
include/
  BikeInoBase.h      # Base structs (BikeInoSettings)
  screens/           # Screen headers
  ride/              # BikeRide header
  utils/             # Utility headers
  lv_conf.h          # LVGL configuration
```
