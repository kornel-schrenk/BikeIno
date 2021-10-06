#ifndef BikeRide_h
#define BikeRide_h

#include <Arduino.h>
#include <M5ez.h>
#include <ezTime.h>
#include <TinyGPS++.h>

// External interface to the obtain GPS data
struct GpsData {
    bool valid = false;
    double latitude = 0.0;
    double longitude = 0.0;
    double currentSpeed = 0.0;    // In km/h
};

//Internal representation of the current bike ride data
struct RideData {
    double latitude = 0.0;
    double longitude = 0.0;
    double currentSpeed = 0.0;
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
    void _debugRideData(RideData* rideData);
};

#endif