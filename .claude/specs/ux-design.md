# Introduction

The screen resolution is 320 x 240 (width x height) pixels. It is a touch screen. There are three buttons available for navigation on the bottom of the screen. 

DiveIno consists of the following screens:

* Ride
* Sensor
* Logbook
* Summary
* Settings

Every screen has two panels:
1. Left hand side content pane
2. Right hand side navigation pane

# Navigation pane

On the top of the navigation screen the battery indicator gets display. Below this indicator each screen has an associated icon. It gets highlighted, if the screen is the current active screen.

For instance at the application start the Ride screen is displayed by default. The icon, which belongs to the Ride screen is active (highlighted), the rest is inactive.

## Battery indicator

The battery level can be one of the followings:
1. 0-10% - Critical - Red
2. 10-25% - Low - Orange
3. 25-75% - Normal - Light green
4. 75%-100% - Charged - Dark green

The battery indicator is always visible and its status is constantly updated by the system.

# Ride screen

The Ride screen is the main screen. It gets displayed when the application starts. It has three panes:
1. Top pane for the GPS or Hall sensor indicator
2. Middle pane for the current speed display
3. Bottom pane for the additional information

## Top pane

It has two icons:
1. GPS indicator icon
2. Hall sensor icon

It indicates, which sensor is active. It can be turned on/off with a touch. Both sensors can be active at the same time. In this case Hall sensor takes the priority.

## Middle pane

It occupies most of the screen. The main goal is to provide optimal visibility for the cyclist even in harsh weather conditions. It displays the current speed in a large monospace font. The speed measurement - km/h or mp/h - will be diplayed in small fonts next to the current speed.

## Bottom pane

This pane displays additional in scrollable fashion:
1. Current time
2. Current temperature
3. Total distance
4. Trip duration
5. Trip distance

# Sensor screen

There are two speed measurement sensors in the system:
1. Hall sensor
2. GPS sensor

The Hall sensor can measure when the wheel spins. It calculates the wheel RPM (Rotation Per Minute). This data will be displayed on this screen, if the Hall sensor is active.

The GPS sensor can measure the current speed and the current location. If the GPS is active, the following data gets displayed:
1. Current speed
2. Position:
  1. Latitude
  2. Longitude
  3. Elavation

The Sensor screen can have two panes:
1. Top pane reserved for the Hall sensor related data
2. Bottom pane reserved for the GPS sensor related data

## Hall sensor pane

It has an indicator icon, which is active when the Hall sensor is active. Beside the icon the current RPM information gets displayed.

## GPS sensor pane

It has an indicator icon, which is active when the GPS sensor is active. Beside the icon the current GPS data gets displayed.

# Logbook screen

This screen has two modes:
1. List mode
2. Details mode

## List mode

In this mode all trips get displayed in a list. The list is scrollable and single selectable.

One list item contains the Trip start date and time plus the total distance of the trip. 

If one Trip gets selected, the Details mode will be activated and the Trips details will be visible on the screen. 

## Details mode

On this screen the Trip details will be shown:
1. Start date and time
2. Stop date and time
3. Duration in hours:minutes:seconds
4. Total distance 
5. Average speed

There are two action buttons:
1. Go back to the List mode
2. Delete the Trip

In case of Trip deletion a Confirmation dialog has to be displayed.

# Summary screen

In this screen the some statistics will be shown for all Trips (rides) were done with BikeIno:
1. Total distance covered
2. Total ride time
3. Number of the Trips
4. Average Trip distance
5. Average Trip duration
6. Average Trip speed

The screen is basically a list, where each statistics is a list item.

# Settings screen

On this screen the following settings will be displayed:
1. Sound on/off
2. Metric/imperial units selection
3. Wheel tyre type selection
4. Delete all trip data
5. Factory reset

The *Sound on/off* and the *Metric/imperial units* selection are toggle buttons.
The *Delete all trip data* and the *Factory reset* are buttons. In this case a confirmation dialog has to be displayed.

The *Wheel tyre type selection* is a drop down list, which contains the following values:
44-559: 26 x 1.75
50-559: 26 x 2.0
55-559: 26 x 2.125
57-584: 27.5 x 2.25
25-622: 700 x 25C / 28 x 1.00
28-622: 700 x 28C / 28 x 1.10
32-622: 700 x 32C / 28 x 1.25
35-622: 700 x 35C / 28 x 1.35
40-622: 700 x 38C / 28 x 1.50
47-622: 700 x 47C / 29 x 1.75
58-622: 29 x 2.35