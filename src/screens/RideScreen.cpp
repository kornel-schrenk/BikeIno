#include "screens/RideScreen.h"

RideScreen::RideScreen(BikeRide* bikeRide)
{
    _bikeRide = bikeRide;
}

void RideScreen::_displayGpsIcon(bool validGpsLocation)
{
    if (validGpsLocation)
    {       
    } else {
    }  
}

void RideScreen::_displayRideDuration(unsigned int rideDuration)
{
}

void RideScreen::init(BikeInoSettings bikeInoSettings, TinyGPSPlus gps)
{        
    this->_displayGpsIcon(gps.location.isValid());
}

int RideScreen::display(TinyGPSPlus gps)
{
    this->_displayGpsIcon(gps.location.isValid());
   
    return STAY_ON_SCREEN;
}

int RideScreen::handleButtonPress(String buttonName, TinyGPSPlus gps)
{
    if (buttonName == "Start") {
        this->_bikeRide->startRide(gps.speed.kmph(), gps.location.lat(), gps.location.lng(), gps.altitude.meters());
        return STAY_ON_SCREEN;
    }
    else if (buttonName == "Stop") {
        this->_bikeRide->stopRide();
        return SCREEN_SUMMARY;
    } 
    else if (buttonName == "Pause") {
        this->_bikeRide->pauseRide();
        return STAY_ON_SCREEN;
    } 
    else if (buttonName == "Restart") {
        this->_bikeRide->restartRide();
        return STAY_ON_SCREEN;
    }
    return STAY_ON_SCREEN;
}