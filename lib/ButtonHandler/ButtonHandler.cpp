#include "ButtonHandler.h"
#include "LedControl.h" 
#include "MqttHandler.h"
#include "BleHandler.h"
#include <Arduino.h>
#include "../../include/PinConfig.h"
#include "LedControlConstants.h"

void IRAM_ATTR statusBtnTriggered() {

  if (keepBlinking) {
    keepBlinking = false;

    if (checkMqttStatus()){
      fadeToColor(ColorSettings::GREEN, 100);
    } else {
      fadeToColor(ColorSettings::RED, 100); 
    }

    turnOffBle();
  } else {
    if (checkMqttStatus()){
      fadeInAndOutColor(ColorSettings::RED, 500);
    } else {
      fadeInAndOutColor(ColorSettings::GREEN, 500);
    }
    
    beginBLE();
  }
}

void setupStatusButtonInterrupt() {
    pinMode(statusBTNPin, INPUT_PULLUP);
    
}

void attachStatusButtonInterrupt(){
    attachInterrupt(statusBTNPin, statusBtnTriggered, FALLING);
}
