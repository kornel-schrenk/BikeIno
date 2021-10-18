#ifndef RideScreen_h
#define RideScreen_h

#include "Arduino.h"
#include "M5ez.h"
#include "ezTime.h"
#include "TinyGPS++.h"

#include "ride/BikeRide.h"
#include "screens/BikeInoScreen.h"
#include "utils/TimeUtils.h"

class RideScreen : public BikeInoScreen {

    public:       
        RideScreen(BikeRide* bikeRide);

        void init(BikeInoSettings bikeInoSettings, TinyGPSPlus gps);

        int display(TinyGPSPlus gps);

        void handleButtonPress(String buttonName, TinyGPSPlus gps);

    private:        
        BikeInoSettings _bikeInoSettings;
        BikeRide* _bikeRide;
        
        void _displayGpsIcon(bool validGpsLocation);
        void _displayRideDuration(unsigned int rideDuration);        
};

#endif