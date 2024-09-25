#include "ButtonHandler.h"
#include "LedControl.h" 
#include "MqttHandler.h"
#include "BleHandler.h"
#include <Arduino.h>
#include "../../include/PinConfig.h"
#include "LedControlConstants.h"

volatile bool statusButtonPressed = false;

void IRAM_ATTR statusBtnTriggered() {
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();

  if (interrupt_time - last_interrupt_time > 200) {
    statusButtonPressed = true;
  }
  last_interrupt_time = interrupt_time;
}

void handleStatusButtonActions(){
  if (!statusButtonPressed){
    return;
  }
  
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

  statusButtonPressed = false;
}

void setupStatusButtonInterrupt() {
  pinMode(statusBTNPin, INPUT_PULLUP);
}

void attachStatusButtonInterrupt(){
    attachInterrupt(statusBTNPin, statusBtnTriggered, FALLING);
}
