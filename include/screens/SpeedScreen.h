#ifndef SpeedScreen_h
#define SpeedScreen_h

#include "Arduino.h"
#include "TinyGPS++.h"

#include "screens/BikeInoScreen.h"

class SpeedScreen : public BikeInoScreen {

    public:
        void init(BikeInoSettings bikeInoSettings);
        
        void display(TinyGPSPlus gps);
        
        void handleButtonPress(String buttonName);                

    private:
        double _previousSpeed = 0.0;
};

#endif