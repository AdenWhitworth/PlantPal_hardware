#include <Arduino.h>
#include "utilities.h"
#include "Storage.h"
#include "LedControl.h"
#include "MqttHandler.h"
#include "SoilSensor.h"
#include "WifiHandler.h"
#include "ButtonHandler.h"
#include "SoilAssessment.h"
#include "LedControlConstants.h"
#include "../../include/PinConfig.h"

void setup() {
  Serial.begin(115200);

  vTaskDelay(pdMS_TO_TICKS(4000));
  
  initRgbLed();
  setupStatusButtonInterrupt();
  beginBlinking(ColorSettings::WHITE);
  checkFirmware();
  loadWifiCredentials(ssid, password);
  vTaskDelay(pdMS_TO_TICKS(5000));
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
