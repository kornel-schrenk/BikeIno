#ifndef BikeInoScreen_h
#define BikeInoScreen_h

#define STAY_ON_SCREEN    456
#define SCREEN_RIDE       300
#define SCREEN_SENSOR     310
#define SCREEN_LOGBOOK    320
#define SCREEN_SUMMARY    330
#define SCREEN_SETTINGS   340

#include <lvgl.h>
#include "BikeInoBase.h"
#include "ui/UIConstants.h"

class BikeInoScreen : public BikeInoBase {

public:
    BikeInoScreen();
    virtual ~BikeInoScreen();

    // Create the LVGL screen and UI elements
    virtual void createUI();

    // Destroy the LVGL screen and clean up
    virtual void destroyUI();

    // Update the screen with current data (called in main loop)
    virtual void update();

    // Get the LVGL screen object
    lv_obj_t* getScreen();

    // Get the content pane (240px left side)
    lv_obj_t* getContentPane();

    // Check if screen is created
    bool isCreated();

protected:
    lv_obj_t* _screen;        // LVGL screen object
    lv_obj_t* _contentPane;   // Content pane (240px width)
    BikeInoSettings _settings;

    // Helper to create the base screen layout
    void createBaseLayout();
};

#endif
