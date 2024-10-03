/**
 * @file SoilAssessmentConstants.h
 * @brief Constants for soil assessment intervals in the PlantPal project.
 * 
 * This file defines constant values used for scheduling soil 
 * assessments and automatic watering intervals.
 */
#ifndef SOIL_ASSESSMENT_CONSTANTS_H
#define SOIL_ASSESSMENT_CONSTANTS_H

namespace AssessmentSettings {
    const unsigned long LOG_INTERVAL_HOURS = 4; // Interval for logging soil assessments in hours.
    const unsigned long AUTO_INTERVAL_HOURS = 1; // Interval for automatic watering assessments in hours.
    const unsigned long LOG_INTERVAL = LOG_INTERVAL_HOURS * 60 * 60 * 1000; // Interval for logging soil assessments in milliseconds.
    const unsigned long AUTO_INTERVAL = AUTO_INTERVAL_HOURS * 60 * 60 * 1000; // Interval for automatic watering assessments in milliseconds.
}

#endif
