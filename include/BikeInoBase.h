#ifndef BikeInoBase_h
#define BikeInoBase_h

struct BikeInoSettings {
	bool soundSetting = false;
	bool imperialUnitsSetting = false;
	uint8_t wheelTyreIndex = 4; // Default: 700 x 25C
};

// StreamBack data from MQTT
struct StreamBackData {
	double latitude = 0.0;
	double longitude = 0.0;
	double speed = 0.0;            // km/h
	double altitude = 0.0;         // meters
	unsigned long timestamp = 0;   // UTC timestamp (ms)
	unsigned long duration = 0;    // ms since stream start
	unsigned long lastReceived = 0; // Local millis() when last received
	bool isValid = false;          // True if data received within timeout
};

class BikeInoBase {

    public: 
    
    private:
};

#endif