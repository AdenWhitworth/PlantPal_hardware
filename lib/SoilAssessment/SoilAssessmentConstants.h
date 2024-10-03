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
    /**
     * @brief Interval for logging soil assessments in hours.
     * 
     * This constant defines the interval at which scheduled 
     * soil assessments will be logged.
     */
    const unsigned long LOG_INTERVAL_HOURS = 4;

    /**
     * @brief Interval for automatic watering assessments in hours.
     * 
     * This constant defines the interval at which automatic 
     * watering assessments will occur.
     */
    const unsigned long AUTO_INTERVAL_HOURS = 1;

    /**
     * @brief Interval for logging soil assessments in milliseconds.
     * 
     * This constant converts the log interval from hours to 
     * milliseconds for scheduling purposes.
     */
    const unsigned long LOG_INTERVAL = LOG_INTERVAL_HOURS * 60 * 60 * 1000;

    /**
     * @brief Interval for automatic watering assessments in milliseconds.
     * 
     * This constant converts the automatic watering interval from 
     * hours to milliseconds for scheduling purposes.
     */
    const unsigned long AUTO_INTERVAL = AUTO_INTERVAL_HOURS * 60 * 60 * 1000;
}

#endif
