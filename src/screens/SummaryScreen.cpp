#include "screens/SummaryScreen.h"

SummaryScreen::SummaryScreen(BikeRide* bikeRide)
{
    _bikeRide = bikeRide;
}

void SummaryScreen::init(BikeInoSettings bikeInoSettings)
{
    ez.screen.clear();
    ez.header.show("Summary");

    ez.buttons.show("Menu");

    ez.canvas.color(ez.theme->foreground);

    M5.Lcd.drawFastHLine(10, 120, 300, ez.theme->foreground);
    M5.Lcd.drawFastVLine(135, 30, 90, ez.theme->foreground);     

    ez.canvas.font(sans26);

    int x = 50;
    int y = 50;

    //Distance
    ez.canvas.pos(x - 40, y - 10);
    ez.canvas.printf("%.1f km", _bikeRide->getDistance());

    //Average speed
    ez.canvas.pos(x - 40, y + 30);
    ez.canvas.printf("%.1f km/h", _bikeRide->getAverageSpeed());

    //Ride start date time
    ez.canvas.pos(x, y + 90);
    ez.canvas.print(_bikeRide->getRideStartDateTime());

    //Ride end date time
    ez.canvas.pos(x, y + 130);
    ez.canvas.print(_bikeRide->getRideEndDateTime());

    ez.canvas.font(numonly48);

    //Duration - hour:min:sec
    ez.canvas.pos(x + 95, y);

    unsigned int elapsedTimeInSeconds = _bikeRide->getDuration();            
    int hours = elapsedTimeInSeconds / 3600;
    elapsedTimeInSeconds = elapsedTimeInSeconds % 3600;
    int minutes = elapsedTimeInSeconds / 60;
    int seconds = elapsedTimeInSeconds % 60;
        
    ez.canvas.printf("%1d:%02d:%02d", hours, minutes, seconds);    
}

void SummaryScreen::display()
{    
}

void SummaryScreen::handleButtonPress(String buttonName)
{
}