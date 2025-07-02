#include "Preferences.h"
#include "screens/HomeScreen.h"

void HomeScreen::initHomeScreen(BikeInoSettings bikeInoSettings) 
{ 
  _bikeInoSettings = bikeInoSettings;  

}

int HomeScreen::handleButtonPress(String buttonName)
{
  if (buttonName == "Update")
  {
    initHomeScreen(_bikeInoSettings);
  }
  else if (buttonName == "Ride")
  {    
    return SCREEN_RIDE;
  }
  return STAY_ON_SCREEN;
}