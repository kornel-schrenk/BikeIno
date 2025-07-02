#include "screens/SpeedScreen.h"

void SpeedScreen::init(BikeInoSettings bikeInoSettings)
{

}

void SpeedScreen::display(TinyGPSPlus gps)
{    
    if (gps.speed.isUpdated())
    {
        double currentSpeed = gps.speed.kmph();
        if (_previousSpeed != currentSpeed) {

        }
    }
}

void SpeedScreen::handleButtonPress(String buttonName)
{
}