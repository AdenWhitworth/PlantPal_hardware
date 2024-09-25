#include <Arduino.h>
#include "utilities.h"
#include "Encryption.h"
#include "LedControl.h"
#include "MqttHandler.h"
#include "SoilSensor.h"
#include "WifiHandler.h"
#include "ButtonHandler.h"
#include "SoilAssessment.h"
#include "LedControlConstants.h"

void setup() {
  Serial.begin(115200);
  delay(1000);

  setupStatusButtonInterrupt();
  fadeToColor(ColorSettings::WHITE, 100); 
  loadAndDecrypt();
  connectToWiFi();
  attachStatusButtonInterrupt();
}

void loop() {
  mqttLoop();
  
  delay(10);
  
  handleStatusButtonActions();

  retrieveShadowOnMqttConnection();

  unsigned long currentMillis = millis();

  scheduledSoilAssessment(currentMillis);
  scheduledAutoWaterAssessment(currentMillis);
  manualWaterAssessment();
}
