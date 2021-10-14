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
    this->_rideData->altitude = gpsData.altitude;
    this->_rideData->averageSpeed = 0.0;
    this->_rideData->distance = 0.0;
    this->_rideData-> durationInSeconds = 0;
    this->_rideData->rideTimestamp = ez.clock.tz.now();     

    //Start GPX output
    String dateTime = ez.clock.tz.dateTime("Y-m-d-H:i:s.v");
    this->_rideLogFilePath = "/RideIno/" + ez.clock.tz.dateTime("YmdHisv") + "-ride.gpx";
    
    String gpxOpenText = _gpx.getOpen();    
    _gpx.setName(this->_rideLogFilePath);
    _gpx.setDesc("RideIno track on " + dateTime);

    String gpxTrackOpenText = _gpx.getTrackOpen();
    String gpxInfoText = _gpx.getInfo();
    String gpxTrackSegmentOpenText = _gpx.getTrackSegmentOpen();

    //SD card output    
    _fileUtils.openRideLog(this->_rideLogFilePath);

    Serial.println(this->_rideLogFilePath + F(" was opened"));

    _fileUtils.appendRideLog(this->_rideLogFilePath, gpxOpenText);
    _fileUtils.appendRideLog(this->_rideLogFilePath, gpxTrackOpenText);
    _fileUtils.appendRideLog(this->_rideLogFilePath, gpxInfoText);
    _fileUtils.appendRideLog(this->_rideLogFilePath, gpxTrackSegmentOpenText);

    //Serial output for logging
    Serial.print(gpxOpenText);
    Serial.print(gpxTrackOpenText);
    Serial.print(gpxInfoText);
    Serial.println(gpxTrackSegmentOpenText);    
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
        newRideData->altitude = gpsData.altitude;

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
        newRideData->altitude = 0.0;
        newRideData->averageSpeed = 0.0;
    }  

    //Log bike ride to the SD card in GPX format
    String gpxTrackPointText = _gpx.getPt(GPX_TRKPT, String(newRideData->longitude, 6), String(newRideData->latitude, 6), String(newRideData->altitude, 1));
    
    Serial.print(gpxTrackPointText);
    if (gpsData.valid) {
        _fileUtils.appendRideLog(this->_rideLogFilePath, gpxTrackPointText);
    }

    this->_rideData = newRideData;
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

    //Close GPX output
    String gpxTrackSegmentCloseText = _gpx.getTrackSegmentClose();
    String gpxTrackCloseText = _gpx.getTrackClose();
    String gpxCloseText = _gpx.getClose();
    
    //Serial output for logging
    Serial.print(gpxTrackSegmentCloseText);
    Serial.print(gpxTrackCloseText);
    Serial.print(gpxCloseText);

    //SD card output
    _fileUtils.appendRideLog(this->_rideLogFilePath, gpxTrackSegmentCloseText);
    _fileUtils.appendRideLog(this->_rideLogFilePath, gpxTrackCloseText);
    _fileUtils.appendRideLog(this->_rideLogFilePath, gpxCloseText);
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