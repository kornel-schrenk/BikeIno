#ifndef HomeScreen_h
#define HomeScreen_h

#include "Arduino.h"
#include "M5ez.h"
#include "ezTime.h"

#include "screens/BikeInoScreen.h"
#include "utils/TimeUtils.h"

class HomeScreen : public BikeInoScreen {

    public:
        HomeScreen(TimeUtils timeUtils);
        void initHomeScreen(BikeInoSettings bikeInoSettings);
        
        void displayHomeClock();
        void updateTime();
        void updateDate();
        void updateAmPm();

        int handleButtonPress(String buttonName);

    private:        
        BikeInoSettings _bikeInoSettings;
        TimeUtils _timeUtils;
};

#endif