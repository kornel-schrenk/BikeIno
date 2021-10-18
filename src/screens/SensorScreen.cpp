#include "screens/SensorScreen.h"

void SensorScreen::init(BikeInoSettings bikeInoSettings)
{
    ez.screen.clear();
    ez.header.show("Sensor");

    ez.buttons.show("Menu");

    ez.canvas.color(ez.theme->foreground);
    ez.canvas.font(sans26);

    int x = 50;
    int y = 50;

    ez.canvas.pos(x, y);
    ez.canvas.print("Lat:");
    ez.canvas.pos(x, y + 30);
    ez.canvas.print("Lon:");
    ez.canvas.pos(x, y + 60);
    ez.canvas.print("Speed:");
    ez.canvas.pos(x, y + 90);
    ez.canvas.print("Altitude:");
}

void SensorScreen::display(TinyGPSPlus gps)
{
    int x = 50;
    int y = 50;

    if (gps.location.isValid())
    {
        ez.canvas.pos(x + 100, y);
        ez.canvas.printf("%f", gps.location.lat());
        ez.canvas.pos(x + 100, y + 30);
        ez.canvas.printf("%f", gps.location.lng());
    }
    
    if (gps.speed.isValid())
    {
        ez.canvas.pos(x + 100, y + 60);
        ez.canvas.printf("%.3f km/h", gps.speed.kmph());
    }

    if (gps.altitude.isValid())
    {
        ez.canvas.pos(x + 100, y + 90);
        ez.canvas.printf("%.0f m", gps.altitude.meters());
    }
    
    ez.canvas.pos(x - 5, y + 130);
    if (gps.date.isValid())
    {
        ez.canvas.printf("%04d-%02d-%02d ", gps.date.year(), gps.date.month(), gps.date.day());        
    }
    if (gps.time.isValid())
    {        
        ez.canvas.printf(" %02d:%02d:%02d", gps.time.hour(), gps.time.minute(), gps.time.second());
    }
}

void SensorScreen::handleButtonPress(String buttonName)
{
}