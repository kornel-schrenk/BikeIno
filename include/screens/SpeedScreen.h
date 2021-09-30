#ifndef SpeedScreen_h
#define SpeedScreen_h

#include "Arduino.h"
#include "M5ez.h"
#include "ezTime.h"

#include "screens/BikeInoScreen.h"

class SpeedScreen : public BikeInoScreen {

    public:
        void init(BikeInoSettings bikeInoSettings);
        
        void display();
        
        void handleButtonPress(String buttonName);                

    private:
};

#endif