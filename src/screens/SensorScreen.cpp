#include "screens/SensorScreen.h"

void SensorScreen::init(BikeInoSettings bikeInoSettings)
{
    ez.screen.clear();
    ez.header.show("Sensor");

    ez.buttons.show("Menu");
}

void SensorScreen::display()
{
    
}

void SensorScreen::handleButtonPress(String buttonName)
{
}