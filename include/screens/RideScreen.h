#ifndef RideScreen_h
#define RideScreen_h

#include "Arduino.h"
#include "M5ez.h"
#include "ezTime.h"

#include "screens/BikeInoScreen.h"

class RideScreen : public BikeInoScreen {

    public:       
        void init(BikeInoSettings bikeInoSettings);

        int display();

        void handleButtonPress(String buttonName);

    private:        
        BikeInoSettings _bikeInoSettings;
};

#endif