
#include <Arduino.h>

const int pumpControlPin = 12;

void setup() {
    Serial.begin(115200);
    pinMode(pumpControlPin, OUTPUT);
    digitalWrite(pumpControlPin, LOW);
}

void loop() {
    Serial.println("Pump ON");
    digitalWrite(pumpControlPin, HIGH);
    delay(5000);

    Serial.println("Pump OFF");
    digitalWrite(pumpControlPin, LOW);
    delay(5000);
}
