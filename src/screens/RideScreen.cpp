#include "screens/RideScreen.h"

RideScreen::RideScreen(BikeRide* bikeRide)
{
    _bikeRide = bikeRide;
}

GpsData RideScreen::_createGpsData(TinyGPSPlus gps)
{
    GpsData gpsData;
    gpsData.valid = true;

    if (gps.location.isValid() && gps.speed.isValid()) 
    {
        gpsData.latitude = gps.location.lat();
        gpsData.longitude = gps.location.lng();        
        gpsData.currentSpeed = gps.speed.kmph();
    } else {
        gpsData.valid = false;    
    }

    return gpsData;
}

void RideScreen::_displayGpsIcon(bool validGpsLocation)
{
    if (validGpsLocation)
    {
        if (ez.theme->name == "Default") {
            M5.Lcd.drawJpg((uint8_t *)ok_small_jpg, (sizeof(ok_small_jpg) / sizeof(ok_small_jpg[0])), 144, 104, 32, 32);
        } else {
            M5.Lcd.drawJpg((uint8_t *)ok_small_jpg_dark, (sizeof(ok_small_jpg_dark) / sizeof(ok_small_jpg_dark[0])), 144, 104, 32, 32);  
        }          
    } else {
        if (ez.theme->name == "Default") {
            M5.Lcd.drawJpg((uint8_t *)tilt_small_jpg, (sizeof(tilt_small_jpg) / sizeof(tilt_small_jpg[0])), 144, 104, 32, 32);
        } else {
            M5.Lcd.drawJpg((uint8_t *)tilt_small_jpg_dark, (sizeof(tilt_small_jpg_dark) / sizeof(tilt_small_jpg_dark[0])), 144, 104, 32, 32);  
        }          
    }  
}

void RideScreen::_displayRideDuration(unsigned int rideDuration)
{
    //Ride duration display
    unsigned long elapsedTimeInSeconds = rideDuration;            
    int hours = elapsedTimeInSeconds / 3600;
    elapsedTimeInSeconds = elapsedTimeInSeconds % 3600;
    int minutes = elapsedTimeInSeconds / 60;
    int seconds = elapsedTimeInSeconds % 60;

    ez.canvas.pos(130, 40);         
    ez.canvas.print(zeropad(hours, 1));
    ez.canvas.print(":");
    ez.canvas.print(zeropad(minutes, 2));
    ez.canvas.print(":");
    ez.canvas.print(zeropad(seconds, 2));
}

void RideScreen::init(BikeInoSettings bikeInoSettings, TinyGPSPlus gps)
{
    ez.screen.clear();
    ez.header.show("Ride");

    if (_bikeRide->isRideInProgress()) 
    {
        if (_bikeRide->isRidePaused()) {
            ez.buttons.show("Restart # Menu # Stop");
        } else {
            ez.buttons.show("Pause # Menu # Stop");
        }        
    } else {
        ez.buttons.show(" # Menu # Start");
    }

    ez.canvas.color(ez.theme->foreground);

    M5.Lcd.drawFastHLine(10, 120, 300, ez.theme->foreground);
    M5.Lcd.drawFastVLine(110, 30, 90, ez.theme->foreground); 
    M5.Lcd.drawFastVLine(160, 120, 90, ez.theme->foreground); 

    ez.canvas.font(sans16);

    ez.canvas.pos(10, 95);
    ez.canvas.print("Speed - km/h");
    ez.canvas.pos(210, 95);
    ez.canvas.print("Duration");
    ez.canvas.pos(10, 185);
    ez.canvas.print("Distance - km");
    ez.canvas.pos(170, 185);
    ez.canvas.print("Average speed - km/h");

    ez.canvas.font(numonly7seg48);

    ez.canvas.pos(10, 40);
    ez.canvas.print("0");
    ez.canvas.pos(130, 40);
    ez.canvas.print("0:00:00");
    ez.canvas.pos(10, 130);    
    ez.canvas.print("0.0");
    ez.canvas.pos(200, 130);
    ez.canvas.print("0.0");

    _currentGpsData = this->_createGpsData(gps);
    this->_displayGpsIcon(this->_currentGpsData.valid);
}

int RideScreen::display(TinyGPSPlus gps)
{
    this->_currentGpsData = this->_createGpsData(gps);

    this->_displayGpsIcon(this->_currentGpsData.valid);

    if (this->_bikeRide->isRideInProgress())
    {
        if (this->_currentGpsData.valid) 
        {
            ez.canvas.pos(10, 40);
            ez.canvas.printf("%.0f", this->_bikeRide->getCurrentSpeed());
        }

        if (!this->_bikeRide->isRidePaused())
        {
            this->_bikeRide->progressRide(this->_currentGpsData);

            //Ride duration display
            this->_displayRideDuration(this->_bikeRide->getDuration());

            if (this->_currentGpsData.valid) 
            {
                //Ride distance display
                ez.canvas.pos(10, 130);                
                ez.canvas.print(this->_bikeRide->getDistance());
                //Average ride speed display
                ez.canvas.pos(200, 130);
                ez.canvas.print(this->_bikeRide->getAverageSpeed());                
            }
        }
    }
    
    return STAY_ON_SCREEN;
}

void RideScreen::handleButtonPress(String buttonName)
{
    if (buttonName == "Start") {
        this->_bikeRide->startRide(this->_currentGpsData);
        ez.buttons.show("Pause # Menu # Stop");
    }
    else if (buttonName == "Stop") {
        this->_bikeRide->stopRide();

        //TODO Navigate to the Ride Summary Screen
    } 
    else if (buttonName == "Pause") {
        this->_bikeRide->pauseRide();
        ez.buttons.show("Restart # Menu # Stop");
    } 
    else if (buttonName == "Restart") {
        this->_bikeRide->restartRide();
        ez.buttons.show("Pause # Menu # Stop");
    }
}