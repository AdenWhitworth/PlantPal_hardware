/**
 * @file MqttHandlerConstants.h
 * @brief Constants for MQTT connection settings.
 * 
 * This header file defines constants that are used for 
 * managing MQTT connections in the ESP32 project. 
 * It includes settings for maximum connection retries 
 * and the delay between connection attempts.
 */
#ifndef MQTT_HANDLER_CONSTANTS_H
#define MQTT_HANDLER_CONSTANTS_H

/**
 * @brief Namespace for MQTT connection settings.
 * 
 * This namespace contains constants related to MQTT connection 
 * retries and delays.
 */
namespace MqttSettings {
    const int MAX_RETRIES = 5; // Maximum number of attempts to connect to the MQTT broker.
    const int RETRY_DELAY_SECONDS = 1; // Delay duration between connection attempts in seconds.
    const int RETRY_DELAY = RETRY_DELAY_SECONDS * 1000; // Delay duration between connection attempts in milliseconds.
}

#endif
