#include "screens/SpeedScreen.h"

void SpeedScreen::init(BikeInoSettings bikeInoSettings)
{
    ez.screen.clear();
    ez.header.show("Speed");

    ez.buttons.show("Menu");
    ez.canvas.color(ez.theme->foreground);

    ez.canvas.font(sans26);
    ez.canvas.pos(130, 170);
    ez.canvas.print("km/h");

    ez.canvas.font(numonly7seg96);
    ez.canvas.pos(80, 60);  
    ez.canvas.printf("%.0f", _previousSpeed);
}

void SpeedScreen::display(TinyGPSPlus gps)
{    
    if (gps.speed.isUpdated())
    {
        double currentSpeed = gps.speed.kmph();
        if (_previousSpeed != currentSpeed) {
            ez.canvas.pos(80, 60);            
            ez.canvas.printf("%.0f", currentSpeed);
            _previousSpeed = currentSpeed;
        }
    }
}

void SpeedScreen::handleButtonPress(String buttonName)
{
}