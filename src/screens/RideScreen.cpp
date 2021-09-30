#include "screens/RideScreen.h"

void RideScreen::init(BikeInoSettings bikeInoSettings)
{
    ez.screen.clear();
    ez.header.show("Ride");

    ez.buttons.show("Menu");
}

int RideScreen::display()
{
    return SCREEN_HOME;
}

void RideScreen::handleButtonPress(String buttonName)
{
}