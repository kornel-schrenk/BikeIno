#ifndef SensorScreen_h
#define SensorScreen_h

#include "Arduino.h"
#include "TinyGPS++.h"

#include "screens/BikeInoScreen.h"

class SensorScreen : public BikeInoScreen {

    public:
        void init(BikeInoSettings bikeInoSettings);
        
        void display(TinyGPSPlus gps);
        
        void handleButtonPress(String buttonName);                

    private:
};

#endif