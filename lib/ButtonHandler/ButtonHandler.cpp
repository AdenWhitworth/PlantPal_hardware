#include "ButtonHandler.h"
#include "LedControl.h" 
#include "MqttHandler.h"
#include "BleHandler.h"
#include <Arduino.h>
#include "../../include/PinConfig.h"
#include "LedControlConstants.h"
#include "ButtonHandlerConstants.h"

volatile bool statusButtonPressed = false;

void IRAM_ATTR statusBtnTriggered() {
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();

  if (interrupt_time - last_interrupt_time > ButtonSettings::DEBOUNCE_DELAY_MILLISECONDS) {
    statusButtonPressed = true;
  }
  last_interrupt_time = interrupt_time;
}

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

void setupStatusButtonInterrupt() {
  pinMode(statusBTNPin, INPUT_PULLUP);
}

void attachStatusButtonInterrupt(){
    attachInterrupt(statusBTNPin, statusBtnTriggered, FALLING);
}
