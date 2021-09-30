#ifndef BikeInoScreen_h
#define BikeInoScreen_h

#define STAY_ON_SCREEN    456
#define SCREEN_HOME       310
#define SCREEN_RIDE       320
#define SCREEN_SPEED      330
#define SCREEN_SENSOR     340
#define SCREEN_LOGBOOK    350
#define SCREEN_SETTINGS   360
#define SCREEN_ABOUT      370

#include "images/jpgs.h"
#include "images/jpgsdark.h"

#include "BikeInoBase.h"

class BikeInoScreen : public BikeInoBase {

    public: 
        void refreshClockWidget();
        bool isClockWidgetDisplayed();
    
    private:
};

#endif