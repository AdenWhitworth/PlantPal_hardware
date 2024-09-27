#include <Arduino.h>
#include "BLEHandler.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "WifiHandler.h"
#include "Storage.h"
#include "../../src/utilities.h"

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
bool deviceConnected = false;

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
    Serial.println("Device connected");
  }

  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
    Serial.println("Device disconnected and BLE turned off");
  }
};

void turnOffBle() {
  Serial.println("Turning off BLE");
  stopAdvertising();
  if (pServer) {
    pServer->disconnect(0);
  }
}

void startAdvertising() {
  Serial.println("Starting BLE advertising...");
  BLEDevice::getAdvertising()->start();
}

void stopAdvertising() {
  Serial.println("Stopping BLE advertising...");
  BLEDevice::getAdvertising()->stop();
}

class MyCallbacks: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string value = pCharacteristic->getValue();

    if (value.length() > 0) {
      
      String valueString = value.c_str();

      int ssidKeyPosition = valueString.indexOf("SSID:");
      int passKeyPosition = valueString.indexOf("PASS:");

      if (ssidKeyPosition != -1 && passKeyPosition != -1) {
          int ssidStart = ssidKeyPosition + 5;
          int ssidEnd = valueString.indexOf(';', ssidStart);
          
          int passStart = passKeyPosition + 5;
          int passEnd = valueString.length();

          if (ssidEnd != -1 && passStart > ssidEnd) { 
              ssid = valueString.substring(ssidStart, ssidEnd);
              password = valueString.substring(passStart, passEnd);

              Serial.println("WiFi SSID: " + ssid);
              Serial.println("WiFi Password: " + password);
              pCharacteristic->setValue("Successfully received wifi credentials");
              pCharacteristic->notify();
          } else {
              Serial.println("Error: Could not parse SSID and Password correctly.");
              pCharacteristic->setValue("Failed to receive wifi credentials");
              pCharacteristic->notify();
          }
      } else {
          Serial.println("Error: SSID or PASS keys not found in the string.");
          pCharacteristic->setValue("Failed to receive wifi credentials");
          pCharacteristic->notify();
      }
    }

    storeWifiCredentials(ssid, password);
    turnOffBle();
    connectToWiFi();
  }
};

void beginBLE() {

  Serial.println("Begin BLE for: ");
  Serial.println(catNum);

  BLEDevice::init(catNum);
  
  BLESecurity *pSecurity = new BLESecurity();
  pSecurity->setAuthenticationMode(ESP_LE_AUTH_REQ_SC_ONLY);

  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(BLEUUID((uint16_t)0xFFE0));
  pCharacteristic = pService->createCharacteristic(
    BLEUUID((uint16_t)0xFFE1),
    BLECharacteristic::PROPERTY_READ |
    BLECharacteristic::PROPERTY_WRITE |
    BLECharacteristic::PROPERTY_NOTIFY
  );

  pCharacteristic->setCallbacks(new MyCallbacks());
  pCharacteristic->addDescriptor(new BLE2902());

  pService->start();
  pServer->getAdvertising()->start();
}
