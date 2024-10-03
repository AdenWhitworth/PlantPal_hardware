#include "SoilSensor.h"

#ifndef MQTT_HANDLER_H
#define MQTT_HANDLER_H"

// External variables to manage the state of the system
extern bool shadow_auto;         // Indicates whether the auto state is enabled.
extern bool shadow_pump;         // Indicates whether the pump state is enabled.
extern bool checkMQTTShadow;     // Flag to check if the shadow state should be retrieved.
extern bool checkInterupt;       // Flag to check for interrupts.

/**
 * @brief Logs an error message to the Serial console.
 * 
 * @param context The context or topic where the error occurred.
 * @param errorMessage The error message to be logged.
 */
void logError(const String& context, const String& errorMessage);

/**
 * @brief Retrieves the initial shadow state from the AWS IoT.
 * 
 * Publishes a request to get the current shadow state.
 */
void getAndCheckShadowState();

/**
 * @brief Updates the reported state of the shadow.
 * 
 * Publishes the current values of the shadow_auto and shadow_pump 
 * variables to the shadow state.
 */
void updateShadowReportedState();

/**
 * @brief Logs soil sensor data to the MQTT broker.
 * 
 * @param currentSoilResponse The current soil sensor response 
 * containing temperature and capacitive values.
 * @param didWater Indicates whether watering occurred during the 
 * soil reading.
 */
void logSoilSensor(soilSensorResponse currentSoilResponse, bool didWater);

/**
 * @brief Handles incoming MQTT messages.
 * 
 * Processes the received MQTT message based on the topic.
 * 
 * @param topic The topic from which the message was received.
 * @param payload The payload of the received message.
 */
void messageHandler(String &topic, String &payload);

/**
 * @brief Connects to the MQTT broker.
 * 
 * Configures the connection settings and attempts to connect to the 
 * specified MQTT broker.
 */
void connectToMQTT();

/**
 * @brief Maintains the MQTT connection by processing incoming messages.
 * 
 * Should be called periodically to ensure proper message handling 
 * and connection maintenance.
 */
void mqttLoop();

/**
 * @brief Checks the current connection status of the MQTT client.
 * 
 * @return True if the client is connected; otherwise, false.
 */
bool checkMqttStatus();

/**
 * @brief Retrieves the shadow state upon a successful MQTT connection.
 * 
 * Calls the function to get the shadow state if the Wi-Fi is 
 * connected and the shadow state retrieval flag is set.
 */
void retrieveShadowOnMqttConnection();

/**
 * @brief Disconnects from the MQTT broker.
 * 
 * Sends a disconnect command to the MQTT client and logs the 
 * disconnection status.
 */
void disconnectFromMQTT();

/**
 * @brief Handles stray MQTT disconnections.
 * 
 * Checks if the MQTT client is disconnected and handles the situation 
 * accordingly. Logs the event and updates the blinking color if 
 * applicable.
 */
void handleStrayMqttDisconnect();

#endif

