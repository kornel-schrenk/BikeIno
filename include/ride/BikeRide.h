#ifndef BikeRide_h
#define BikeRide_h

#include <Arduino.h>
#include <M5ez.h>
#include <ezTime.h>
#include <TinyGPS++.h>
#include "utils/GPX.h"
#include "utils/FileUtils.h"

// External interface to the obtain GPS data
struct GpsData {
    bool valid = false;
    double latitude = 0.0;
    double longitude = 0.0;
    double currentSpeed = 0.0;    // In km/h
    double altitude = 0.0;
};

//Internal representation of the current bike ride data
struct RideData {
    double latitude = 0.0;
    double longitude = 0.0;
    double currentSpeed = 0.0;
    double altitude = 0.0;
    double averageSpeed = 0.0;
    double distance = 0.0;
    unsigned int durationInSeconds = 0;
    time_t rideTimestamp;
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

    // Lifecycle methods //

    void startRide(GpsData gpsData);
    void progressRide(GpsData gpsData);
    void pauseRide();
    void restartRide();
    void stopRide();

private:
    bool _rideInProgress = false;
    bool _pausedRide = false;

    RideData* _rideData;
    GPX _gpx;
    FileUtils _fileUtils;
    String _rideLogFilePath;

    void _debugRideData(RideData* rideData);
};

#endif