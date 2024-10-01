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
String bleResponseMessage = "";
bool recievedNewCredentials = false;

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
    Serial.println("Device connected");
  }

  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
    Serial.println("Device disconnected");
  }
};

void turnOffBle() {
  Serial.println("Stopping BLE...");

  BLEDevice::stopAdvertising();

  if (deviceConnected) {
    pServer->disconnect(0);
    deviceConnected = false;
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
  
  BLEDevice::deinit();
  btStop();
  esp_bt_controller_disable();
  
  Serial.println("BLE stopped and memory released.");
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
          bleResponseMessage = "Successfully received wifi credentials";
        } else {
          Serial.println("Error: Could not parse SSID and Password correctly.");
          bleResponseMessage = "Failed to receive wifi credentials";
        }
      } else {
        Serial.println("Error: SSID or PASS keys not found in the string.");
        bleResponseMessage = "Failed to receive wifi credentials";
      }
    }

    storeWifiCredentials(ssid, password);
    checkDisconnections();
    pCharacteristic->setValue(bleResponseMessage.c_str());
    pCharacteristic->notify();

    vTaskDelay(pdMS_TO_TICKS(4000));

    recievedNewCredentials = true;
  }
};

void handleNewCredentials() {
  if (recievedNewCredentials){
    turnOffBle();

    vTaskDelay(pdMS_TO_TICKS(4000));

    connectToWiFi();
    recievedNewCredentials = false;
  }
}

void beginBLE() {

  Serial.println("Begin BLE for: ");
  Serial.println(catNum);

  BLEDevice::init(catNum);
  
  BLESecurity *pSecurity = new BLESecurity();
  pSecurity->setAuthenticationMode(ESP_LE_AUTH_REQ_SC_ONLY);

  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ |
    BLECharacteristic::PROPERTY_WRITE |
    BLECharacteristic::PROPERTY_NOTIFY
  );

  pCharacteristic->setCallbacks(new MyCallbacks());
  pCharacteristic->addDescriptor(new BLE2902());

  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
}
