#ifndef BikeRide_h
#define BikeRide_h

#include <Arduino.h>
#include <M5ez.h>
#include <ezTime.h>
#include <TinyGPS++.h>
#include "utils/GPX.h"
#include "utils/FileUtils.h"

//Internal representation of the current bike ride data
struct RideData {
    double latitude = 0.0;
    double longitude = 0.0;
    double currentSpeed = 0.0;
    double altitude = 0.0;
    double averageSpeed = 0.0;
    double distance = 0.0;
    unsigned long rideTimestamp;
    unsigned int durationInSeconds = 0;
    String rideStartDateTime = "";
    String rideEndDateTime = "";        
};

class BikeRide
{
public:
    bool isRideInProgress();
    bool isRidePaused();

    double getCurrentSpeed();
    unsigned int getDuration();
    double getDistance();
    double getAverageSpeed();
    String getRideStartDateTime();
    String getRideEndDateTime();    

    // Lifecycle methods //

    void startRide(double currentSpeed, double latitude, double longitude, double altitude);
    void progressRide(bool isValidLocation, double currentSpeed, double latitude, double longitude, double altitude, String utcTime);
    void pauseRide();
    void restartRide();
    void stopRide();

private:
    bool _rideInProgress = false;
    bool _pausedRide = false;

    RideData _rideData;
    GPX _gpx;
    FileUtils _fileUtils;
    String _rideLogFilePath;
};

#endif