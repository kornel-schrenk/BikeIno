#ifndef RideScreen_h
#define RideScreen_h

#include "Arduino.h"
#include "M5ez.h"
#include "ezTime.h"
#include "TinyGPS++.h"

#include "ride/BikeRide.h"
#include "screens/BikeInoScreen.h"

class RideScreen : public BikeInoScreen {

    public:       
        RideScreen(BikeRide* bikeRide);

        void init(BikeInoSettings bikeInoSettings, TinyGPSPlus gps);

        int display(TinyGPSPlus gps);

        void handleButtonPress(String buttonName);

    private:        
        BikeInoSettings _bikeInoSettings;
        BikeRide* _bikeRide;
        GpsData _currentGpsData;
        
        GpsData _createGpsData(TinyGPSPlus gps);
        void _displayGpsIcon(bool validGpsLocation);
        void _displayRideDuration(unsigned int rideDuration);        
};

#endif