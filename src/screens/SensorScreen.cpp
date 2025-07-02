#include "screens/SensorScreen.h"

void SensorScreen::init(BikeInoSettings bikeInoSettings)
{

}

void SensorScreen::display(TinyGPSPlus gps)
{
    int x = 50;
    int y = 50;

    if (gps.location.isValid())
    {
    }
    
    if (gps.speed.isValid())
    {
    }

    if (gps.altitude.isValid())
    {
    }
    
    if (gps.date.isValid())
    {
    }
    if (gps.time.isValid())
    {        
    }
}

void SensorScreen::handleButtonPress(String buttonName)
{
}