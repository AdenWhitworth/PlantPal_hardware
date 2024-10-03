#include "SoilSensor.h"
#ifndef MQTT_HANDLER_H
#define MQTT_HANDLER_H

extern bool shadow_auto;
extern bool shadow_pump;
extern bool checkMQTTShadow; 
extern bool checkInterupt;
void logError(const String& context, const String& errorMessage);
void getAndCheckShadowState();
void updateShadowReportedState();
void logSoilSensor(soilSensorResponse currentSoilResponse, bool didWater);
void messageHandler(String &topic, String &payload);
void connectToMQTT();
void mqttLoop();
bool checkMqttStatus();
void retrieveShadowOnMqttConnection();
void disconnectFromMQTT();
void handleStrayMqttDisconnect();

#endif
