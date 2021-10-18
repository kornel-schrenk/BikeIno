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
    return _rideData.currentSpeed;
}

unsigned int BikeRide::getDuration()
{
    return _rideData.durationInSeconds;
}

double BikeRide::getDistance()
{
    return _rideData.distance;
}

double BikeRide::getAverageSpeed()
{
    return _rideData.averageSpeed;
}

void BikeRide::startRide(double currentSpeed, double latitude, double longitude, double altitude)
{
    //Serial.printf("START Speed: %.1f km/h Lat: %f Lon: %f Ela: %.0f m\n", currentSpeed, latitude, longitude, altitude);

    this->_rideInProgress = true;
    this->_pausedRide = false;
   
    _rideData.latitude = latitude;
    _rideData.longitude = longitude;
    _rideData.currentSpeed = currentSpeed;
    _rideData.altitude = altitude;
    _rideData.averageSpeed = 0.0;
    _rideData.distance = 0.0;
    _rideData.durationInSeconds = 0;
    _rideData.rideTimestamp = millis();     

    //Start GPX output
    this->_rideLogFilePath = "/RideIno/" + ez.clock.tz.dateTime("YmdHisv") + "-ride.gpx";
        
    _gpx.setName(this->_rideLogFilePath);
    _gpx.setDesc("RideIno track on " + ez.clock.tz.dateTime("Y-m-d-H:i:s.v"));

    String gpxOutput = _gpx.getOpen() + _gpx.getTrackOpen() + _gpx.getInfo() + _gpx.getTrackSegmentOpen();

    //SD card output    
    _fileUtils.openRideLog(this->_rideLogFilePath);

    Serial.println(this->_rideLogFilePath + F(" was opened"));

    _fileUtils.appendRideLog(this->_rideLogFilePath, gpxOutput);

    //Serial output for logging
    Serial.println(gpxOutput);    
}
    
void BikeRide::progressRide(bool isValidLocation, double currentSpeed, double latitude, double longitude, double altitude, String utcTime)
{           
    //Ride duration calculation
    unsigned int timeDifferenceMilliSeconds = (millis() - _rideData.rideTimestamp);          
    int duration = timeDifferenceMilliSeconds / 1000;    
    //Serial.printf("PROGRESS Duration: %i Valid: %s Speed: %.1f km/h Lat: %f Lon: %f Ela: %.0f m\n", durationInSeconds, isValidLocation?"true":"false", currentSpeed, latitude, longitude, altitude);

    if (isValidLocation) 
    {
        _rideData.latitude = latitude;
        _rideData.longitude = longitude;
        _rideData.currentSpeed = currentSpeed;
        _rideData.altitude = altitude;

        if (duration > _rideData.durationInSeconds) {
            //Log bike ride to the SD card in GPX format
            String gpxTrackPointText = 
                _gpx.getPt(GPX_TRKPT, String(_rideData.longitude, 6), String(_rideData.latitude, 6), String(_rideData.altitude, 1), utcTime);    
            Serial.print(gpxTrackPointText);
            _fileUtils.appendRideLog(this->_rideLogFilePath, gpxTrackPointText); 

            double distanceCovered =  _rideData.currentSpeed *  (duration - _rideData.durationInSeconds)/ 3600;
            _rideData.durationInSeconds = duration;       

            // //Average speed calculation
            if (distanceCovered != 0.0) {
                _rideData.distance = _rideData.distance + distanceCovered;
                _rideData.averageSpeed = _rideData.distance * 3600 / _rideData.durationInSeconds;        
            }                
        }
    } else {
        //If the GPS location is not valid stop the ride on the last valid position
        _rideData.currentSpeed = 0.0;
    }  
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
    this->_pausedRide = false;

    //Close GPX output
    String gpxOutput = _gpx.getTrackSegmentClose() + _gpx.getTrackClose() + _gpx.getClose();

    //SD card output
    _fileUtils.appendRideLog(this->_rideLogFilePath, gpxOutput);
    
    //Serial output for logging
    Serial.println(gpxOutput);
}
