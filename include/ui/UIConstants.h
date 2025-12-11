#ifndef UIConstants_h
#define UIConstants_h

#include <lvgl.h>

// Layout dimensions
#define SCREEN_WIDTH        320
#define SCREEN_HEIGHT       240
#define CONTENT_PANE_WIDTH  260
#define NAV_PANE_WIDTH      60

// Battery indicator colors
#define COLOR_BATTERY_CRITICAL  lv_color_hex(0xE53935)  // Red (0-10%)
#define COLOR_BATTERY_LOW       lv_color_hex(0xFF9800)  // Orange (10-25%)
#define COLOR_BATTERY_NORMAL    lv_color_hex(0x8BC34A)  // Light green (25-75%)
#define COLOR_BATTERY_CHARGED   lv_color_hex(0x4CAF50)  // Dark green (75-100%)

// UI colors
#define COLOR_ACTIVE            lv_color_hex(0x2196F3)  // Blue (active icon/element)
#define COLOR_INACTIVE          lv_color_hex(0x757575)  // Gray (inactive icon/element)
#define COLOR_BACKGROUND        lv_color_hex(0x212121)  // Dark background
#define COLOR_NAV_BACKGROUND    lv_color_hex(0x303030)  // Navigation pane background
#define COLOR_TEXT_PRIMARY      lv_color_hex(0xFFFFFF)  // White text
#define COLOR_TEXT_SECONDARY    lv_color_hex(0xB0B0B0)  // Gray text

// Navigation pane icon size
#define NAV_ICON_SIZE           32
#define NAV_ICON_PADDING        8

// Ride screen layout
#define RIDE_TOP_PANE_HEIGHT    30
#define RIDE_MIDDLE_PANE_HEIGHT 140
#define RIDE_BOTTOM_PANE_HEIGHT 70

// Info tile dimensions
#define INFO_TILE_WIDTH         80
#define INFO_TILE_HEIGHT        70

// Wheel tyre circumferences in mm (for speed calculation from Hall sensor)
static const uint16_t WHEEL_CIRCUMFERENCES[] = {
    2070,  // 44-559: 26 x 1.75
    2090,  // 50-559: 26 x 2.0
    2115,  // 55-559: 26 x 2.125
    2200,  // 57-584: 27.5 x 2.25
    2105,  // 25-622: 700 x 25C / 28 x 1.00
    2136,  // 28-622: 700 x 28C / 28 x 1.10
    2155,  // 32-622: 700 x 32C / 28 x 1.25
    2168,  // 35-622: 700 x 35C / 28 x 1.35
    2180,  // 40-622: 700 x 38C / 28 x 1.50
    2268,  // 47-622: 700 x 47C / 29 x 1.75
    2326   // 58-622: 29 x 2.35
};

// Wheel tyre display names
static const char* WHEEL_TYRE_NAMES =
    "26 x 1.75\n"
    "26 x 2.0\n"
    "26 x 2.125\n"
    "27.5 x 2.25\n"
    "700 x 25C\n"
    "700 x 28C\n"
    "700 x 32C\n"
    "700 x 35C\n"
    "700 x 38C\n"
    "700 x 47C\n"
    "29 x 2.35";

#define WHEEL_TYRE_COUNT 11

#endif
