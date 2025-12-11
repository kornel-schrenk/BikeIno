#ifndef SummaryScreen_h
#define SummaryScreen_h

#include "Arduino.h"

#include "ride/BikeRide.h"
#include "screens/BikeInoScreen.h"

// Statistics data structure
struct TripStatistics {
    double totalDistance;
    unsigned long totalDuration;
    unsigned int tripCount;
    double avgDistance;
    double avgDuration;
    double avgSpeed;
};

class SummaryScreen : public BikeInoScreen {

public:
    SummaryScreen(BikeRide* bikeRide);

    // Override base class methods
    void createUI() override;
    void destroyUI() override;
    void update() override;

    // Set settings
    void setSettings(BikeInoSettings settings);

private:
    BikeRide* _bikeRide;
    TripStatistics _stats;

    // Statistics labels
    lv_obj_t* _titleLabel;
    lv_obj_t* _totalDistLabel;
    lv_obj_t* _totalTimeLabel;
    lv_obj_t* _tripCountLabel;
    lv_obj_t* _avgDistLabel;
    lv_obj_t* _avgTimeLabel;
    lv_obj_t* _avgSpeedLabel;

    void _createStatsList();
    void _loadStatistics();
    void _updateLabels();

    // Helper to create a stat row
    lv_obj_t* _createStatRow(lv_obj_t* parent, const char* label, const char* value, int yPos);
};

#endif
