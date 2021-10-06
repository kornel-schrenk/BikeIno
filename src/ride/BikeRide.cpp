#include "ride/BikeRide.h"

bool BikeRide::isRideInProgress()
{
    return this->_rideInProgress;
}

bool BikeRide::isRidePaused()
{
    return this->_pausedRide;
}

double BikeRide::getCurrentSpeed()
{
    return this->_rideData->currentSpeed;
}

unsigned int BikeRide::getDuration()
{
    return this->_rideData->durationInSeconds;
}

double BikeRide::getDistance()
{
    return this->_rideData->distance;
}

double BikeRide::getAverageSpeed()
{
    return this->_rideData->averageSpeed;
}

void BikeRide::startRide(GpsData gpsData)
{
    this->_rideInProgress = true;
    this->_pausedRide = false;

    this->_rideData = new RideData;    
    this->_rideData->latitude = gpsData.latitude;
    this->_rideData->longitude = gpsData.longitude;
    this->_rideData->currentSpeed = gpsData.currentSpeed;
    this->_rideData->averageSpeed = 0.0;
    this->_rideData->distance = 0.0;
    this->_rideData-> durationInSeconds = 0;
    this->_rideData->rideTimestamp = ez.clock.tz.now();    
}
    
void BikeRide::progressRide(GpsData gpsData)
{       
    RideData* newRideData = new RideData;

    //Ride duration calculation
    newRideData->rideTimestamp = ez.clock.tz.now();
    unsigned int timeDifferenceSeconds = (newRideData->rideTimestamp - _rideData->rideTimestamp);    
    newRideData->durationInSeconds = (_rideData->durationInSeconds + timeDifferenceSeconds);

    if (gpsData.valid) 
    {
        newRideData->latitude = gpsData.latitude;
        newRideData->longitude = gpsData.longitude;
        newRideData->currentSpeed = gpsData.currentSpeed;

        //Ride distance calculation
        double distanceCovered = 0.0;
        if (_rideData->latitude != 0.0 && _rideData->longitude != 0.0) {
            distanceCovered = 
                TinyGPSPlus::distanceBetween(_rideData->latitude, _rideData->longitude, newRideData->latitude, newRideData->longitude);
        } 
        newRideData->distance = (_rideData->distance + (distanceCovered / 1000));

        //Average speed calculation
        newRideData->averageSpeed = distanceCovered / (newRideData->durationInSeconds / 3600);
    } else {
        //If the GPS data is not valid stop the ride on the last valid position
        newRideData->latitude = _rideData->latitude;
        newRideData->longitude = _rideData->longitude;
        newRideData->distance = _rideData->distance;
        newRideData->currentSpeed = 0.0;
        newRideData->averageSpeed = 0.0;
    }  

    //TODO Log bike ride to the SD card in GPX format

    this->_rideData = newRideData;
    this->_debugRideData(this->_rideData);
}

void BikeRide::pauseRide()
{
    this->_pausedRide = true;
}

void BikeRide::restartRide()
{
    this->_pausedRide = false;
}

void BikeRide::stopRide()
{
    this->_rideInProgress = false;
}

void BikeRide::_debugRideData(RideData* rideData)
{
    Serial.println(F("\nCurrent Ride data:"));
    Serial.printf("latitude: %f ", rideData->latitude);
    Serial.printf("longitude: %f\n", rideData->longitude);
    Serial.printf("current speed: %f ", rideData->currentSpeed);
    Serial.printf("average speed: %f\n", rideData->averageSpeed);
    Serial.printf("distance: %f\n", rideData->distance);
    Serial.printf("duration: %d ", rideData->durationInSeconds);    
    Serial.printf("timestamp: %lu\n", rideData->rideTimestamp);    
}