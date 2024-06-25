#include <Arduino.h>
#include <SPI.h>
#include "Adafruit_seesaw.h"
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <MQTT.h>
#include <ArduinoJson.h>
#include <utilities.h>

Adafruit_seesaw soilsensor;
WiFiClientSecure net = WiFiClientSecure();
MQTTClient client = MQTTClient(256);

/* Pump Control */
int pumpControlPin = 9;

/* Buttons */
const byte statusBTNPin = 34;

/* RGB LED */
const byte GreenLedPin = 32;
const byte RedLedPin = 26;
const byte BlueLedPin = 27;
#define PWM1_Ch    0
#define PWM1_Res   8
#define PWM1_Freq  1000
#define PWM2_Ch    1
#define PWM2_Res   8
#define PWM2_Freq  1000
#define PWM3_Ch    2
#define PWM3_Res   8
#define PWM3_Freq  1000

/* Global Variables */
int maxSoilCapacitive = 2000; //very wet soil
int minSoilCapacitive = 200; // very dry soil
int targetSoilCapacitive = 1200; //watered to this point
int triggerSoilCapacitive = 600; //begin watering at this point

struct soilSensorResponse {
  float soilTemperature;
  uint16_t soilCapacitive;
};

/* Init Functions */

bool initSoilSensor(){

  static bool isFirstCall = true;

  if (isFirstCall){
    if (!soilsensor.begin(0x36)) {
    Serial.println("Couldnt find Adafruit Soil Sensor!");
      return false;
    }
    Serial.print("Seesaw Soil Sensor started! version: ");
    Serial.println(soilsensor.getVersion(), HEX);
    isFirstCall = false;
  }
  return true;
}

bool initStatusButton(){

  static bool isFirstCall = true;

  if (isFirstCall){
    
    pinMode(statusBTNPin, INPUT_PULLUP);
	  attachInterrupt(statusBTNPin, statusBtnTriggered, FALLING);
      
    Serial.print("Successfully attached status button interrupt");
    isFirstCall = false;
  }
  return true;
}

/* Sensor Functions */

soilSensorResponse readSoilSensor(soilSensorResponse currentSoilResponse) {
  
  currentSoilResponse = {
    soilsensor.getTemp(),
    soilsensor.touchRead(0)
  };

  return currentSoilResponse;

}

bool assessSoil(){

  soilSensorResponse currentSoilResponse;
  currentSoilResponse = readSoilSensor(currentSoilResponse);
  Serial.println("Soil Temp: " + String(currentSoilResponse.soilTemperature));
  Serial.println("Soil Cap: " + String(currentSoilResponse.soilCapacitive));

  if (int(currentSoilResponse.soilCapacitive) < triggerSoilCapacitive){
    return true;
  } else {
    return false;
  }

}

/* Pump Functions */

void pumpWater(int maxPumpSpeed, int rampDelay, int pumpDuration){

  static bool isFirstCall = true;

  if (isFirstCall){
    pinMode(pumpControlPin, OUTPUT);
    isFirstCall = false;
  }

  if ( maxPumpSpeed > 255){
    return;
  }
  
  //ramp up pump
  for(int pumpSpeed = 0; pumpSpeed <= maxPumpSpeed; pumpSpeed++){
    analogWrite(pumpControlPin, pumpSpeed);
    delay(rampDelay);
  }

  delay(pumpDuration);

  //ramp down pump
  for(int pumpSpeed = 0; pumpSpeed >= 0; pumpSpeed--){
    analogWrite(pumpControlPin, pumpSpeed);
    delay(rampDelay);
  }   

}

void correctSoilCapacitive(){

  soilSensorResponse currentSoilResponse;
  currentSoilResponse = readSoilSensor(currentSoilResponse);

  while (int(currentSoilResponse.soilCapacitive) < targetSoilCapacitive && int(currentSoilResponse.soilCapacitive) > triggerSoilCapacitive){

    pumpWater(80,5,1000);

    currentSoilResponse = readSoilSensor(currentSoilResponse);

  }

}

/* MQTT */

void setupWifi(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
}

void messageHandler(String &topic, String &payload) {
  Serial.println("received:");
  Serial.println("- topic: " + topic);
  Serial.println("- payload:");
  Serial.println(payload);

  StaticJsonDocument<200> doc;
  deserializeJson(doc, payload);
  const char* message = doc["message"];
  Serial.println(message);
  
}

void connectToMQTT() {

  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);

  client.begin(AWS_IOT_ENDPOINT, 8883, net);

  client.onMessage(messageHandler);

  Serial.print("ESP32 - Connecting to MQTT broker");

    while (!client.connect(THINGNAME)) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();

  if (!client.connected()) {
    Serial.println("ESP32 - AWS IoT Timeout!");
    return;
  }

  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);

  Serial.println("ESP32  - AWS IoT Connected!");
}

void sendToMQTT() {
  StaticJsonDocument<200> message;
  message["timestamp"] = millis();
  message["data"] = "HelloAWS"; 
  char messageBuffer[512];
  serializeJson(message, messageBuffer);

  client.publish(AWS_IOT_PUBLISH_TOPIC, messageBuffer);

  Serial.println("sent:");
  Serial.print("- topic: ");
  Serial.println(AWS_IOT_PUBLISH_TOPIC);
  Serial.print("- payload:");
  Serial.println(messageBuffer);
}

/* LED */

void setColor(int redValue, int greenValue, int blueValue){

  static bool isFirstCall = true;

  if (isFirstCall){
    ledcAttachPin(RedLedPin, PWM1_Ch);
    ledcSetup(PWM1_Ch, PWM1_Freq, PWM1_Res);
    
    ledcAttachPin(GreenLedPin, PWM2_Ch);
    ledcSetup(PWM2_Ch, PWM2_Freq, PWM2_Res);
    
    ledcAttachPin(BlueLedPin, PWM3_Ch);
    ledcSetup(PWM3_Ch, PWM3_Freq, PWM3_Res);
    isFirstCall = false;
  }
  
  if (redValue != 256){
    redValue = 255 - redValue;
  }
  if (greenValue != 256){
    greenValue = 255 - greenValue;
  }
  if (blueValue != 256){
    blueValue = 255 - blueValue;
  }
  
  ledcWrite(0,redValue); 
  ledcWrite(1,greenValue);
  ledcWrite(2,blueValue);
}

void flashLed(int redColorNumber, int greenColorNumber, int blueColorNumber, bool cycleRed, bool cycleGreen, bool cycleBlue, int fadeSpeed, int holdColorDelay, bool fadeIn, bool fadeOut, int fadeMultiple, int loopFadeDelay, bool turnOff){
  //RGB Number breakdown
  //255 is full color
  //0 is white
  //256 is off

  for (int i = 1; i <= fadeMultiple; i++){

    if(fadeIn){
      for(int dutyCycle = 0; dutyCycle <= 255; dutyCycle++){   
      
        if (cycleRed){
          setColor(dutyCycle,greenColorNumber,blueColorNumber);
        }

        if (cycleGreen){
          setColor(redColorNumber,dutyCycle,blueColorNumber);
        }

        if (cycleBlue){
          setColor(redColorNumber,greenColorNumber,dutyCycle);
        }
        
        delay(fadeSpeed);
      }
    }
    
    if (holdColorDelay!= 0){
      delay(holdColorDelay);
    }

    if(fadeOut){
      for(int dutyCycle = 0; dutyCycle <= 255; dutyCycle++){
        int reverseDudyCycle = 255- dutyCycle;    
      
        if (cycleRed){
          setColor(reverseDudyCycle,greenColorNumber,blueColorNumber);
        }

        if (cycleGreen){
          setColor(redColorNumber,reverseDudyCycle,blueColorNumber);
        }

        if (cycleBlue){
          setColor(redColorNumber,reverseDudyCycle,dutyCycle);
        }
        
        delay(fadeSpeed);
      }
    }
    
    if(turnOff){
      setColor(256,256,256);
    }

    if (loopFadeDelay!= 0){
      delay(loopFadeDelay);
    }
  }
}

/* Status Button */

void IRAM_ATTR statusBtnTriggered() {
	Serial.println("Status button pressed");
}


void setup() {
  Serial.begin(115200);

  //initSoilSensor();

  setupWifi();

  connectToMQTT();

  sendToMQTT();

}

void loop() {
  client.loop();
}
