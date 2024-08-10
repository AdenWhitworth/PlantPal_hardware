#include <Arduino.h>
#include <SPI.h>
#include "Adafruit_seesaw.h"
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <MQTT.h>
#include <ArduinoJson.h>
#include <utilities.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <AESLib.h>
#include <Preferences.h>

Adafruit_seesaw soilsensor;
WiFiClientSecure net = WiFiClientSecure();
MQTTClient client = MQTTClient(1024);
bool shadow_auto;
bool shadow_pump;
bool checkMQTTShadow = false; 

/* BLE */
BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
bool deviceConnected = false;
const char* ssid = nullptr;
const char* password = nullptr;

/* Encryp & Storage */
AESLib aesLib;

const byte aesKey[] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0xcf, 0x5a, 0xe4, 0x4a, 0x3e, 0x2e}; // 16 bytes key
byte aesIV[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f}; // 16 bytes IV

Preferences preferences;

/* Pump Control */
int pumpControlPin = 9;

/* Buttons */
const byte statusBTNPin = 34;

/* RGB LED */
const byte GreenLedPin = 32;
const byte RedLedPin = 26;
const byte BlueLedPin = 27;
bool keepBlinking = false;
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

void getAndCheckShadowState() {
  Serial.println("Retrieving initial shadow state...");
  
  if (client.publish(AWS_IOT_SHADOW_GET, "{}")) {
    Serial.println("Shadow get request published");
  } else {
    Serial.println("Failed to publish shadow get request");
  }
}

void updateShadowReportedState() {
  StaticJsonDocument<256> doc;
  JsonObject state = doc.createNestedObject("state");
  JsonObject reported = state.createNestedObject("reported");
  reported["auto"] = shadow_auto;
  reported["pump"] = shadow_pump;

  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer);

  client.publish(AWS_IOT_SHADOW_UPDATE, jsonBuffer);

  Serial.println("sent:");
  Serial.print("- topic: ");
  Serial.println(AWS_IOT_SHADOW_UPDATE);
  Serial.print("- payload:");
  Serial.println(jsonBuffer);
}

void messageHandler(String &topic, String &payload) {
  Serial.println("received:");
  Serial.println("- topic: " + topic);
  Serial.println("- payload:");
  Serial.println(payload);

  StaticJsonDocument<1024> doc;
  deserializeJson(doc, payload);

  if (topic == AWS_IOT_SHADOW_UPDATE_DELTA){
    JsonObject shadow_state = doc["state"];

    if (shadow_state.containsKey("auto")){
      shadow_auto = shadow_state["auto"];
    }

    if (shadow_state.containsKey("pump")){
      shadow_pump = shadow_state["pump"];
    }

    Serial.println("shadow_auto: " + String(shadow_auto));
    Serial.println("shadow_pump: " + String(shadow_pump));
    updateShadowReportedState();
    return;
  }

  if (topic == AWS_IOT_SHADOW_ACCEPTED){
    JsonObject shadow_state = doc["state"];
    JsonObject shadow_desired = shadow_state["desired"];
    JsonObject shadow_reported = shadow_state["reported"];
  
    if (shadow_desired.containsKey("auto") && shadow_desired["auto"] != shadow_reported["auto"]) {
      shadow_auto = shadow_desired["auto"];
    }

    if (shadow_desired.containsKey("pump") && shadow_desired["pump"] != shadow_reported["pump"]) {
      shadow_pump = shadow_desired["pump"];
    }

    updateShadowReportedState();
    return;
  }

  if (topic == AWS_IOT_SHADOW_REJECTED){
    Serial.println("Failed to get shadow document");
    return;
  }


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

  client.subscribe(AWS_IOT_SHADOW_ACCEPTED);
  client.subscribe(AWS_IOT_SHADOW_REJECTED);
  client.subscribe(AWS_IOT_SHADOW_UPDATE_DELTA);

  Serial.println("ESP32  - AWS IoT Connected!");
  checkMQTTShadow = true; 
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

void fadeToColor(int red, int green, int blue, int fadeDuration) {
    for (int i = 0; i <= 255; i++) {
        setColor(red * i / 255, green * i / 255, blue * i / 255);
        delay(fadeDuration);
    }
}

void fadeInAndOutColor(int red, int green, int blue, int fadeDuration) {
    while (keepBlinking) {
        for (int i = 0; i <= 255; i++) {
            if (!keepBlinking) return;
            setColor(red * i / 255, green * i / 255, blue * i / 255);
            delay(fadeDuration);
        }
        // Fade out to black
        for (int i = 255; i >= 0; i--) {
            if (!keepBlinking) return; 
            setColor(red * i / 255, green * i / 255, blue * i / 255);
            delay(fadeDuration);
        }
    }
}

/* Data Storage */
void encryptAndStore(const char* ssid, const char* password) {
    int ssidLen = strlen(ssid);
    int passwordLen = strlen(password);

    byte encryptedSSID[64];
    byte encryptedPassword[128];

    aesLib.encrypt((byte*)ssid, ssidLen, encryptedSSID, aesKey, sizeof(aesKey), aesIV);

    aesLib.encrypt((byte*)password, passwordLen, encryptedPassword, aesKey, sizeof(aesKey), aesIV);

    preferences.begin("wifi", false);
    preferences.putBytes("ssid", encryptedSSID, sizeof(encryptedSSID));
    preferences.putBytes("password", encryptedPassword, sizeof(encryptedPassword));
    preferences.end();
}

void loadAndDecrypt() {
    byte encryptedSSID[64];
    byte encryptedPassword[128];

    preferences.begin("wifi", true);
    size_t ssidLength = preferences.getBytes("ssid", encryptedSSID, sizeof(encryptedSSID));
    size_t passwordLength = preferences.getBytes("password", encryptedPassword, sizeof(encryptedPassword));
    preferences.end();

    char ssidBuffer[32];
    char passwordBuffer[64];
    
    aesLib.decrypt(encryptedSSID, ssidLength, (byte*)ssidBuffer, aesKey, sizeof(aesKey), aesIV);
    aesLib.decrypt(encryptedPassword, passwordLength, (byte*)passwordBuffer, aesKey, sizeof(aesKey), aesIV);

    ssid = strdup(ssidBuffer);
    password = strdup(passwordBuffer);
}

/* BLE */

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
    Serial.println("Device connected");
  }

  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
    Serial.println("Device disconnected");
    // Start advertising again
    //pServer->startAdvertising();
    //Serial.println("Started advertising again");
  }
};

void connectToWiFi() {
    if (ssid && password) {
        Serial.println("Attempting to connect to WiFi...");
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid, password);

        int maxRetries = 10;
        int attempt = 0;
        while (WiFi.status() != WL_CONNECTED && attempt < maxRetries) {
            delay(1000);
            Serial.print(".");
            attempt++;
        }

        if (WiFi.status() == WL_CONNECTED) {
            Serial.println("\nConnected to WiFi");
            keepBlinking = false;
            setColor(0, 255, 0);
            connectToMQTT();
        } else {
            Serial.println("\nFailed to connect to WiFi");
            keepBlinking = false;
            setColor(255, 0, 0); 
        }
    }
}

class MyCallbacks: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string value = pCharacteristic->getValue();
    if (value.length() > 0) {
      if (value.find("SSID:") == 0) {
        ssid = value.substr(5).c_str();
      } else if (value.find("PASS:") == 0) {
        password = value.substr(5).c_str();
      }
    }

    if (ssid && password) {
      encryptAndStore(ssid, password);
      connectToWiFi();
      if (WiFi.status() == WL_CONNECTED){
        pCharacteristic->setValue("Connected to WiFi!");
        pCharacteristic->notify();
      } else {
        pCharacteristic->setValue("Failed to connect to WiFi.");
        pCharacteristic->notify();
      }
    }
  }
};

void beginBLE() {
  BLEDevice::init("ESP32_BLE");
  
  BLESecurity *pSecurity = new BLESecurity();
  pSecurity->setAuthenticationMode(ESP_LE_AUTH_REQ_SC_ONLY);

  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(BLEUUID((uint16_t)0xFFE0));
  pCharacteristic = pService->createCharacteristic(
                      BLEUUID((uint16_t)0xFFE1),
                      BLECharacteristic::PROPERTY_READ |
                      BLECharacteristic::PROPERTY_WRITE
                    );

  pCharacteristic->setCallbacks(new MyCallbacks());
  pCharacteristic->addDescriptor(new BLE2902());

  pService->start();
  pServer->getAdvertising()->start();
}

void IRAM_ATTR statusBtnTriggered() {
    detachInterrupt(statusBTNPin);
    fadeInAndOutColor(255,0,0,500);
    beginBLE();
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(statusBTNPin, INPUT_PULLUP);

  setColor(255,0,0);

  loadAndDecrypt();
  connectToWiFi();
  
  if (WiFi.status() != WL_CONNECTED) {
	  attachInterrupt(statusBTNPin, statusBtnTriggered, FALLING);
  }

}

void loop() {
  client.loop();
  
  delay(10);
  
  if (WiFi.status() == WL_CONNECTED && checkMQTTShadow) {
    getAndCheckShadowState();
    checkMQTTShadow = false;
  } 
  
  if (WiFi.status() != WL_CONNECTED && !deviceConnected) {
    attachInterrupt(statusBTNPin, statusBtnTriggered, FALLING);
  }

}
