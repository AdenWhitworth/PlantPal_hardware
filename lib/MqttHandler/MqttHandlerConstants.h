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
