/**
 * @file PumpControlConstants.h
 * @brief Constants for pump control settings in the PlantPal project.
 * 
 * This header file defines constant values related to pump operation 
 * settings for the PlantPal project. These constants are used to 
 * configure the duration of pump operation, the maximum number of retry 
 * attempts for pumping, and the delay between retries.
 */

#ifndef PUMP_CONTROL_CONSTANTS_H
#define PUMP_CONTROL_CONSTANTS_H

/**
 * @brief Namespace for pump control settings.
 * 
 * This namespace contains constants used to configure pump operation 
 * parameters, including duration and retry settings.
 */
namespace PumpSettings {
    const int PUMP_DURATION_SECONDS = 3; // Duration for which the pump operates, in seconds.
    const int PUMP_DURATION = PUMP_DURATION_SECONDS * 1000; // Duration for which the pump operates, in milliseconds.
    const int MAX_RETRY_ATTEMPTS = 4; // Maximum number of retry attempts for pumping.
    const int RETRY_DELAY_SECONDS = 2; // Delay between retry attempts, in seconds.
    const int RETRY_DELAY = RETRY_DELAY_SECONDS * 1000; // Delay between retry attempts, in milliseconds.
}

#endif