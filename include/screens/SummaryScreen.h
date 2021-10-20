#ifndef SummaryScreen_h
#define SummaryScreen_h

#include "Arduino.h"
#include "M5ez.h"

#include "ride/BikeRide.h"
#include "screens/BikeInoScreen.h"

class SummaryScreen : public BikeInoScreen {

    public:       
        SummaryScreen(BikeRide* bikeRide);

        void init(BikeInoSettings bikeInoSettings);

        void display();

        void handleButtonPress(String buttonName);

    private:        
        BikeInoSettings _bikeInoSettings;
        BikeRide* _bikeRide;        
};

#endif