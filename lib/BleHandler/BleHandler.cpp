#include <Arduino.h>
#include "BLEHandler.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "WifiHandler.h"
#include "Encryption.h"

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

void turnOffBle (){
  if (ssid && ssid != ""){
    btStop();
    esp_bt_controller_disable();
  }
  
  BLEDevice::deinit(true);
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
      
      if (checkWifiStatus()){
        pCharacteristic->setValue("Connected to WiFi!");
        pCharacteristic->notify();
      } else {
        pCharacteristic->setValue("Failed to connect to WiFi.");
        pCharacteristic->notify();
      }
    }

    turnOffBle();

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
