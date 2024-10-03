#ifndef SOIL_SENSOR_CONSTANTS_H
#define SOIL_SENSOR_CONSTANTS_H

namespace SoilSettings {
    const int MAX_CAPACITIVE = 2000; //very wet soil
    const int MIN_CAPACITIVE = 200; // very dry soil
    const int TARGET_CAPACITIVE = 800; //1200 watered to this point
    const int TRIGGER_CAPACITIVE = 600; //begin watering at this point
}

#endif
