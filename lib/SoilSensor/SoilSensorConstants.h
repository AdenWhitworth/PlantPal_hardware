/**
 * @file SoilSensorConstants.h
 * @brief Header file for soil sensor constants.
 * 
 * This file defines various constants used for soil sensor 
 * capacitive readings to determine soil moisture levels and 
 * watering thresholds.
 */

#ifndef SOIL_SENSOR_CONSTANTS_H
#define SOIL_SENSOR_CONSTANTS_H

/**
 * @namespace SoilSettings
 * @brief Namespace for soil sensor related constants.
 * 
 * This namespace contains constants that represent different 
 * capacitive thresholds for soil moisture assessment.
 */
namespace SoilSettings {
    const int MAX_CAPACITIVE = 2000; // Maximum capacitive value indicating very wet soil.
    const int MIN_CAPACITIVE = 200; // Minimum capacitive value indicating very dry soil.
    const int TARGET_CAPACITIVE = 800; // Target capacitive value indicating the desired soil moisture level.
    const int TRIGGER_CAPACITIVE = 600; // Capacitive value at which watering should begin.
}

#endif
