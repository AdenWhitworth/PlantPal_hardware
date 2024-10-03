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

namespace PumpSettings {
    /**
     * @brief Duration for which the pump operates, in seconds.
     * 
     * This constant defines the duration for which the pump is activated 
     * during its operation.
     */
    const int PUMP_DURATION_SECONDS = 3;

    /**
     * @brief Duration for which the pump operates, in milliseconds.
     * 
     * This constant is derived from PUMP_DURATION_SECONDS and is used 
     * for time-sensitive operations in milliseconds.
     */
    const int PUMP_DURATION = PUMP_DURATION_SECONDS * 1000;

    /**
     * @brief Maximum number of retry attempts for pumping.
     * 
     * This constant specifies how many times the pump will attempt to 
     * activate to achieve the desired soil conditions before stopping.
     */
    const int MAX_RETRY_ATTEMPTS = 4;

    /**
     * @brief Delay between retry attempts, in seconds.
     * 
     * This constant defines the time to wait before attempting to pump 
     * again after a failed attempt.
     */
    const int RETRY_DELAY_SECONDS = 2;

    /**
     * @brief Delay between retry attempts, in milliseconds.
     * 
     * This constant is derived from RETRY_DELAY_SECONDS and is used 
     * for time-sensitive operations in milliseconds.
     */
    const int RETRY_DELAY = RETRY_DELAY_SECONDS * 1000;
}

#endif