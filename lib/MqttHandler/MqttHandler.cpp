#include <Arduino.h>
#include "MQTTHandler.h"
#include <MQTT.h>
#include <WiFiClientSecure.h>
#include "../../src/utilities.h"
#include <ArduinoJson.h>
#include "LedControl.h"
#include "SoilSensor.h"
#include "WifiHandler.h"
#include "LedControlConstants.h"
#include "MqttHandlerConstants.h"

WiFiClientSecure net;
MQTTClient client(1024);

bool shadow_auto;
bool shadow_pump;
bool checkMQTTShadow = false; 
bool checkInterupt = false;
bool wasMqttDisconnected = false;

/**
 * @brief Logs an error message to the serial output.
 * 
 * @param topic The MQTT topic associated with the error.
 * @param errorMessage The error message to log.
 */
void logError(const String& topic, const String& errorMessage) {
  Serial.println();
  Serial.println("*****************************");
  Serial.print("error:");
  Serial.print("- topic:" + topic);
  Serial.print("- message: ");
  Serial.println(errorMessage);
  Serial.println("*****************************");
}

/**
 * @brief Logs a successful sent message to the serial output.
 * 
 * @param topic The MQTT topic to which the message was sent.
 * @param payload The payload of the message sent.
 */
void logSuccessSent(const String& topic, const String& payload){
  Serial.println();
  Serial.println("*****************************");
  Serial.println("sent:");
  Serial.println("- topic: " + topic);
  Serial.print("- payload:");
  Serial.println(payload);
  Serial.println("*****************************");
}

/**
 * @brief Logs a successful received message to the serial output.
 * 
 * @param topic The MQTT topic from which the message was received.
 * @param payload The payload of the received message.
 */
void logSuccessRecieve(const String& topic, const String& payload){
  Serial.println();
  Serial.println("*****************************");
  Serial.println("received:");
  Serial.println("- topic: " + topic);
  Serial.println("- payload:");
  Serial.println(payload);
  Serial.println("*****************************");
}

/**
 * @brief Retrieves the initial shadow state from the AWS IoT.
 * 
 * Sends a request to get the shadow state and logs the result.
 */
void getAndCheckShadowState() {
  Serial.println("Retrieving initial shadow state...");
  
  if (client.publish(AWS_IOT_SHADOW_GET, "{}")) {
    logSuccessSent(AWS_IOT_SHADOW_GET,"Shadow get request published");
  } else {
    logError(AWS_IOT_SHADOW_GET, "Failed to publish shadow get request");
  }
}

/**
 * @brief Updates the reported shadow state on AWS IoT.
 * 
 * Creates a JSON document with the current shadow state and publishes it.
 */
void updateShadowReportedState() {
  StaticJsonDocument<256> doc;
  JsonObject state = doc.createNestedObject("state");
  JsonObject reported = state.createNestedObject("reported");
  
  reported["auto"] = shadow_auto;

  if (shadow_pump){
    reported["pump"] = shadow_pump;
    shadow_pump = false;
  }

  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer);

  client.publish(AWS_IOT_SHADOW_UPDATE, jsonBuffer);

  logSuccessSent(AWS_IOT_SHADOW_UPDATE,jsonBuffer);
}

/**
 * @brief Logs the soil sensor data to AWS IoT.
 * 
 * @param currentSoilResponse The current response from the soil sensor.
 * @param didWater Indicates whether watering occurred.
 */
void logSoilSensor(soilSensorResponse currentSoilResponse, bool didWater) {
  StaticJsonDocument<256> doc;
  doc["cat_num"] = catNum;
  doc["soil_temp"] = currentSoilResponse.soilTemperature;
  doc["soil_cap"] = currentSoilResponse.soilCapacitive;
  doc["water"] = didWater;

  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer);

  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);

  logSuccessSent(AWS_IOT_PUBLISH_TOPIC,jsonBuffer);

  if(!keepBlinking){
    fadeToColor(assessmentColor());
  }
}

/**
 * @brief Handles the shadow update delta from AWS IoT.
 * 
 * @param shadow_state The JSON object containing the shadow state.
 */
void handleShadowUpdateDelta(JsonObject &shadow_state){
  
  Serial.println("***************");
  Serial.println("Shadow Update Delta");
  
  
  bool received_shadow_auto;
  bool received_shadow_pump;

  if (shadow_state.containsKey("auto")){
    received_shadow_auto = shadow_state["auto"].as<int>() == 1;
  }

  if (shadow_state.containsKey("pump")){
    received_shadow_pump = shadow_state["pump"].as<int>() == 1;
  }

  if ( shadow_auto != received_shadow_auto){
    Serial.println("Recieved new desired shadow auto state: " + received_shadow_auto? "True" : "False");
    shadow_auto = received_shadow_auto;
    updateShadowReportedState();
  }

  if (shadow_pump != received_shadow_pump){
    Serial.println("Recieved new desired shadow pump state: " + received_shadow_pump? "True" : "False");
    shadow_pump = received_shadow_pump;
  }

  Serial.println("***************");

  return;
}

/**
 * @brief Handles the accepted shadow update from AWS IoT.
 * 
 * @param shadow_state The JSON object containing the shadow state.
 */
void handleShadowAccepted(JsonObject &shadow_state){
  JsonObject shadow_desired = shadow_state["desired"];
  JsonObject shadow_reported = shadow_state["reported"];
  
  if (shadow_desired.containsKey("auto") && shadow_desired["auto"] != shadow_reported["auto"]) {
    shadow_auto = shadow_desired["auto"].as<int>() == 1;
    updateShadowReportedState();
  }

  if (shadow_desired.containsKey("pump") && shadow_desired["pump"] != shadow_reported["pump"]) {
    shadow_pump = shadow_desired["pump"].as<int>() == 1;
  }

  soilSensorResponse currentSoilResponse;
  currentSoilResponse = readSoilSensor(currentSoilResponse);
  logSoilSensor(currentSoilResponse,false);

  return;
}

/**
 * @brief Handles incoming MQTT messages.
 * 
 * @param topic The MQTT topic of the message.
 * @param payload The payload of the message.
 */
void messageHandler(String &topic, String &payload) {
  logSuccessRecieve(topic, payload);
  StaticJsonDocument<1024> doc;
  DeserializationError error = deserializeJson(doc, payload);

  if (error) {
    logError(topic, "Failed to deserialize JSON: " + String(error.c_str()));
    return;
  }

  if (topic == AWS_IOT_SHADOW_REJECTED){
    logError(topic, "Failed to get shadow document");
    return;
  }

  if (topic == AWS_IOT_SUBSCRIBE_TOPIC && doc["error"]){
    logError(topic, "Failed to log sensor data");
    return;
  }

  JsonObject shadow_state = doc["state"];

  if (topic == AWS_IOT_SHADOW_UPDATE_DELTA){
    handleShadowUpdateDelta(shadow_state);
  }

  if (topic == AWS_IOT_SHADOW_ACCEPTED){
    handleShadowAccepted(shadow_state);
  }
}

/**
 * @brief Connects to the MQTT broker.
 * 
 * Sets up the secure client and attempts to connect to the MQTT broker.
 * Subscribes to necessary topics after a successful connection.
 */
void connectToMQTT() {
  wasMqttDisconnected = false;
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);

  client.begin(AWS_IOT_ENDPOINT, 8883, net);

  client.onMessage(messageHandler);

  Serial.print("ESP32 - Connecting to MQTT broker");

  int attempts = 0;
  while (!client.connect(THINGNAME) && attempts < MqttSettings::MAX_RETRIES) {
      Serial.println("Attempt " + String(attempts) + " to connect to MQTT broker");
      vTaskDelay(pdMS_TO_TICKS(MqttSettings::RETRY_DELAY));
      attempts++;
  }

  if (attempts == MqttSettings::MAX_RETRIES) {
      logError("MQTT Connection", "Exceeded maximum connection attempts");
      return;
  }

  if (!checkMqttStatus()) {
    Serial.println("ESP32 - AWS IoT Timeout!");
    keepBlinking = false;
    fadeToColor(ColorSettings::ORANGE); 
    return;
  }

  if (!client.subscribe(AWS_IOT_SHADOW_ACCEPTED) ||
    !client.subscribe(AWS_IOT_SHADOW_REJECTED) ||
    !client.subscribe(AWS_IOT_SHADOW_UPDATE_DELTA) ||
    !client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC)) {
    logError("MQTT Subscription", "Failed to subscribe to one or more topics");
    return;
  }

  Serial.println("ESP32  - AWS IoT Connected!");
  keepBlinking = false;
  endBlinking(ColorSettings::WHITE);
  checkMQTTShadow = true; 
}

/**
 * @brief Handles the MQTT loop for maintaining the connection.
 * 
 * Calls the MQTT client's loop function if connected, allowing 
 * for message handling and maintaining the connection. 
 * Delays execution for a short period to avoid busy-waiting.
 */
void mqttLoop() {
  if (checkMqttStatus()){
    client.loop();
  
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

/**
 * @brief Checks the connection status of the MQTT client.
 * 
 * @return True if the client is connected; otherwise, false.
 */
bool checkMqttStatus() {
  return client.connected();
}

/**
 * @brief Retrieves the shadow state upon a successful MQTT connection.
 * 
 * Checks if the Wi-Fi is connected and if the shadow state retrieval 
 * flag is set, then calls the function to get and check the shadow state.
 */
void retrieveShadowOnMqttConnection(){
  if (checkWifiStatus() && checkMQTTShadow) {
    getAndCheckShadowState();
    checkMQTTShadow = false;
  } 
}

/**
 * @brief Disconnects from the MQTT broker.
 * 
 * Sends a disconnect command to the MQTT client and delays 
 * for a brief period before confirming disconnection.
 */
void disconnectFromMQTT() {
  Serial.println("Disconnecting from MQTT broker...");
  client.disconnect();
  vTaskDelay(pdMS_TO_TICKS(1000));
  Serial.println("Disconnected from MQTT broker.");
}

/**
 * @brief Handles stray MQTT disconnections.
 * 
 * Checks if the MQTT client is disconnected and handles the situation 
 * accordingly. If disconnected, logs the event and updates the 
 * blinking color if applicable.
 */
void handleStrayMqttDisconnect() {
  if (!checkMqttStatus() && !wasMqttDisconnected){
    Serial.println("MQTT Disconnected");
    wasMqttDisconnected = true;

    if (currentColor == ColorSettings::ORANGE){
      return;
    }

    if (keepBlinking){
      switchBlinkingColor(ColorSettings::ORANGE);
      return;
    }

    fadeToColor(ColorSettings::ORANGE); 

  }
}


