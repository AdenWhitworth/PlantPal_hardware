/**
 * @file main.cpp
 * @brief Main application file for PlantPal ESP32 project.
 *
 * This file serves as the primary entry point for the PlantPal application, 
 * an IoT solution designed to help plant enthusiasts monitor and manage 
 * their plants effectively. The application integrates various hardware 
 * components and communication protocols to provide a comprehensive plant 
 * management system.
 *
 * ## Overview
 * The application utilizes an ESP32 microcontroller to handle multiple 
 * functionalities, including Wi-Fi connectivity, MQTT communication, 
 * soil sensor readings, LED control, and user interactions through a 
 * status button. The program continuously runs in a loop, checking 
 * sensor data and managing watering schedules based on the soil 
 * conditions.
 */
#include <Arduino.h>
#include "utilities.h"
#include "Storage.h"
#include "LedControl.h"
#include "MqttHandler.h"
#include "SoilSensor.h"
#include "WifiHandler.h"
#include "ButtonHandler.h"
#include "SoilAssessment.h"
#include "BleHandler.h"
#include "LedControlConstants.h"
#include "../../include/PinConfig.h"

const int BOOT_DELAY_SECONDS = 4; // Delay in seconds for booting the system
const int BOOT_DELAY = BOOT_DELAY_SECONDS * 1000; // Delay in milliseconds for booting the system
const int SETUP_DELAY_SECONDS = 5; // Delay in seconds for the setup phase
const int SETUP_DELAY = SETUP_DELAY_SECONDS * 1000; // Delay in milliseconds for the setup phase

/**
 * @brief Initializes the system.
 *
 * This function sets up the serial communication, initializes RGB LED,
 * sets up the status button interrupt, blinks the LED, checks firmware
 * versions, loads WiFi credentials, and connects to WiFi.
 * It also includes delays to ensure proper timing during setup.
 */
void setup() {
  Serial.begin(115200);

  vTaskDelay(pdMS_TO_TICKS(BOOT_DELAY));
  
  initRgbLed();
  setupStatusButtonInterrupt();
  beginBlinking(ColorSettings::WHITE);
  checkFirmware();
  loadWifiCredentials(ssid, password);
  vTaskDelay(pdMS_TO_TICKS(SETUP_DELAY));
  connectToWiFi();
  attachStatusButtonInterrupt();
}

/**
 * @brief Main loop of the application.
 *
 * This function continuously runs after the setup function. It handles
 * MQTT communication, checks for stray MQTT disconnects, manages button
 * actions, retrieves the shadow on MQTT connection, and performs scheduled
 * soil assessments and watering tasks.
 */
void loop() {
  
  mqttLoop();
  
  handleStrayMqttDisconnect();

  handleStatusButtonActions();
  
  retrieveShadowOnMqttConnection();

  unsigned long currentMillis = millis();

  scheduledSoilAssessment(currentMillis);
  scheduledAutoWaterAssessment(currentMillis);
  manualWaterAssessment();
  
  handleNewCredentials();
}
