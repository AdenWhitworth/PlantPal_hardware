#include <Arduino.h>

#define statusBTNPin  4

volatile bool buttonPressed = false;

void IRAM_ATTR statusBtnTriggered() {
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();

  if (interrupt_time - last_interrupt_time > 200) {
    buttonPressed = true;
  }
  last_interrupt_time = interrupt_time;
}

void setupStatusButtonInterrupt() {
  pinMode(statusBTNPin, INPUT_PULLUP);
}

void attachStatusButtonInterrupt(){
    attachInterrupt(statusBTNPin, statusBtnTriggered, FALLING);
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Start");

  setupStatusButtonInterrupt();
  delay(50);
  attachStatusButtonInterrupt();

  Serial.println("Attached");
}

void loop() {
  if (buttonPressed) {
    Serial.println("Button Triggered");
    buttonPressed = false;
  }
}