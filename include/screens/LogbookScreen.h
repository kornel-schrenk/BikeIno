#ifndef LogbookScreen_h
#define LogbookScreen_h

#include "Arduino.h"

#include "screens/BikeInoScreen.h"

class LogbookScreen : public BikeInoScreen {

    public:
        void init(BikeInoSettings bikeInoSettings);

        void display();

        void handleButtonPress(String buttonName);

    private:
};

#endif