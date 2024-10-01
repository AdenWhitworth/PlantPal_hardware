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

void logError(const String& topic, const String& errorMessage) {
  Serial.println();
  Serial.println("*****************************");
  Serial.print("error:");
  Serial.print("- topic:" + topic);
  Serial.print("- message: ");
  Serial.println(errorMessage);
  Serial.println("*****************************");
}

void logSuccessSent(const String& topic, const String& payload){
  Serial.println();
  Serial.println("*****************************");
  Serial.println("sent:");
  Serial.println("- topic: " + topic);
  Serial.print("- payload:");
  Serial.println(payload);
  Serial.println("*****************************");
}

void logSuccessRecieve(const String& topic, const String& payload){
  Serial.println();
  Serial.println("*****************************");
  Serial.println("received:");
  Serial.println("- topic: " + topic);
  Serial.println("- payload:");
  Serial.println(payload);
  Serial.println("*****************************");
}

void getAndCheckShadowState() {
  Serial.println("Retrieving initial shadow state...");
  
  if (client.publish(AWS_IOT_SHADOW_GET, "{}")) {
    logSuccessSent(AWS_IOT_SHADOW_GET,"Shadow get request published");
  } else {
    logError(AWS_IOT_SHADOW_GET, "Failed to publish shadow get request");
  }
}

void updateShadowReportedState() {
  StaticJsonDocument<256> doc;
  JsonObject state = doc.createNestedObject("state");
  JsonObject reported = state.createNestedObject("reported");
  
  if (shadow_auto){
    reported["auto"] = shadow_auto;
  }

  if (shadow_pump){
    reported["pump"] = shadow_pump;
    shadow_pump = false;
  }

  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer);

  client.publish(AWS_IOT_SHADOW_UPDATE, jsonBuffer);

  logSuccessSent(AWS_IOT_SHADOW_UPDATE,jsonBuffer);
}

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

void handleShadowUpdateDelta(JsonObject &shadow_state){
  bool recieved_shadow_auto;
  bool recieved_shadow_pump;

  JsonObject shadow_desired = shadow_state["desired"];

  if (shadow_desired.containsKey("auto")){
    recieved_shadow_auto = shadow_desired["auto"].as<int>() == 1;
  }

  if (shadow_desired.containsKey("pump")){
    recieved_shadow_pump = shadow_desired["pump"].as<int>() == 1;
  }

  if ( shadow_auto != recieved_shadow_auto){
    Serial.println("Recieved new desired shadow auto state: " + recieved_shadow_auto? "True" : "False");
    shadow_auto = recieved_shadow_auto;
  }

  if (shadow_pump != recieved_shadow_pump){
    Serial.println("Recieved new desired shadow pump state: " + recieved_shadow_pump? "True" : "False");
    shadow_pump = recieved_shadow_pump;
  }

  return;
}

void handleShadowAccepted(JsonObject &shadow_state){
  JsonObject shadow_desired = shadow_state["desired"];
  JsonObject shadow_reported = shadow_state["reported"];

  if (shadow_desired.containsKey("auto") && shadow_desired["auto"] != shadow_reported["auto"]) {
    shadow_auto = shadow_desired["auto"].as<int>() == 1;
  }

  if (shadow_desired.containsKey("pump") && shadow_desired["pump"] != shadow_reported["pump"]) {
    shadow_pump = shadow_desired["pump"].as<int>() == 1;
  }

  soilSensorResponse currentSoilResponse;
  currentSoilResponse = readSoilSensor(currentSoilResponse);
  logSoilSensor(currentSoilResponse,false);

  return;
}

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
      Serial.print(".");
      vTaskDelay(pdMS_TO_TICKS(1000));
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

  client.subscribe(AWS_IOT_SHADOW_ACCEPTED);
  client.subscribe(AWS_IOT_SHADOW_REJECTED);
  client.subscribe(AWS_IOT_SHADOW_UPDATE_DELTA);
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);

  Serial.println("ESP32  - AWS IoT Connected!");
  keepBlinking = false;
  endBlinking(assessmentColor());
  checkMQTTShadow = true; 
}

void mqttLoop() {
  if (checkMqttStatus()){
    client.loop();
  
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

bool checkMqttStatus() {
  return client.connected();
}

void retrieveShadowOnMqttConnection(){
  if (checkWifiStatus && checkMQTTShadow) {
    getAndCheckShadowState();
    checkMQTTShadow = false;
  } 
}

void disconnectFromMQTT() {
  Serial.println("Disconnecting from MQTT broker...");
  client.disconnect();
  vTaskDelay(pdMS_TO_TICKS(1000));
  Serial.println("Disconnected from MQTT broker.");
}

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


