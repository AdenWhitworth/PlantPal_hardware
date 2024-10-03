/**
 * @file ButtonHandler.cpp
 * @brief Handles the status button functionality for the ESP32 project.
 * 
 * This file contains the implementation of the status button interrupt 
 * service routines and associated actions. It manages button debouncing 
 * and the logic for starting and stopping blinking LED sequences and 
 * initializing or stopping BLE (Bluetooth Low Energy) based on the button state.
 */
#include "ButtonHandler.h"
#include "LedControl.h" 
#include "MqttHandler.h"
#include "BleHandler.h"
#include <Arduino.h>
#include "../../include/PinConfig.h"
#include "LedControlConstants.h"
#include "ButtonHandlerConstants.h"

volatile bool statusButtonPressed = false;

/**
 * @brief Interrupt service routine for the status button.
 * 
 * This function is called when the status button is pressed. 
 * It checks the time since the last interrupt to debounce the 
 * button press and sets the statusButtonPressed flag accordingly.
 */
void IRAM_ATTR statusBtnTriggered() {
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();

  if (interrupt_time - last_interrupt_time > ButtonSettings::DEBOUNCE_DELAY_MILLISECONDS) {
    statusButtonPressed = true;
  }
  last_interrupt_time = interrupt_time;
}

/**
 * @brief Handles actions based on the status button state.
 * 
 * If the status button has been pressed, this function checks 
 * the blinking state. It either stops blinking and turns off 
 * BLE or starts blinking and initializes BLE depending on the 
 * current state.
 */
void handleStatusButtonActions(){
  if (!statusButtonPressed) return;

  if (keepBlinking) {
    Serial.println("Stopping blinking and turning off BLE...");
    endBlinking(currentColor);
    turnOffBle();
  } else {
    if(!keepBlinking){
      Serial.println("Starting blinking and initializing BLE...");
      if (checkMqttStatus()){
        beginBlinking(assessmentColor());
      } else {
        beginBlinking(ColorSettings::ORANGE);
      }
    }
    beginBLE();
  }

  statusButtonPressed = false;
}

/**
 * @brief Sets up the interrupt for the status button.
 * 
 * Configures the status button pin as an input with pull-up 
 * resistor to detect button presses.
 */
void setupStatusButtonInterrupt() {
  pinMode(statusBTNPin, INPUT_PULLUP);
}

/**
 * @brief Attaches the interrupt for the status button.
 * 
 * Configures the interrupt to trigger the statusBtnTriggered 
 * function on a falling edge (button press).
 */
void attachStatusButtonInterrupt(){
    attachInterrupt(statusBTNPin, statusBtnTriggered, FALLING);
}
