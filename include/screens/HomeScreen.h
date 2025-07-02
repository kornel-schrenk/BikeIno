#ifndef HomeScreen_h
#define HomeScreen_h

#include "Arduino.h"

#include "screens/BikeInoScreen.h"

class HomeScreen : public BikeInoScreen {

    public:
        void initHomeScreen(BikeInoSettings bikeInoSettings);

        int handleButtonPress(String buttonName);

    private:        
        BikeInoSettings _bikeInoSettings;
};

#endif