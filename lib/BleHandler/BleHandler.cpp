#include <Arduino.h>
#include "BLEHandler.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "WifiHandler.h"
#include "Storage.h"
#include "../../src/utilities.h"
#include "BleHandlerConstants.h"

BLEServer* pServer = NULL;
BLESecurity *pSecurity = nullptr; 
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

  if (deviceConnected && pServer != nullptr) {
    pServer->disconnect(0);
    deviceConnected = false;
    vTaskDelay(pdMS_TO_TICKS(BleSettings::BLE_DISCONNECT_DELAY));
  }
  
  BLEDevice::deinit();
  btStop();
  esp_bt_controller_disable();

  if (pSecurity != nullptr) {
    delete pSecurity;
    pSecurity = nullptr;
  }
  
  Serial.println("BLE stopped and memory released.");
}

bool parseWifiCredentials(const String& valueString, String& ssid, String& password) {
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
      return true;
    }
  }

  return false;
}

class MyCallbacks: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string value = pCharacteristic->getValue();

    if (value.length() > 0) {
      String valueString = value.c_str();
      if (parseWifiCredentials(valueString, ssid, password)) {
        bleResponseMessage = "Successfully received wifi credentials";
      } else {
        Serial.println("Error: Could not parse SSID and Password correctly.");
        bleResponseMessage = "Failed to receive wifi credentials";
      }
    }

    storeWifiCredentials(ssid, password);
    handleDisconnections();
    pCharacteristic->setValue(bleResponseMessage.c_str());
    pCharacteristic->notify();

    vTaskDelay(pdMS_TO_TICKS(BleSettings::BLE_RESPONSE_DELAY));

    recievedNewCredentials = true;
  }
};

void handleNewCredentials() {
  if (recievedNewCredentials){
    turnOffBle();

    vTaskDelay(pdMS_TO_TICKS(BleSettings::BLE_RESPONSE_DELAY));

    connectToWiFi();
    recievedNewCredentials = false;
  }
}

void beginBLE() {

  Serial.println("Begin BLE for: ");
  Serial.println(catNum);

  BLEDevice::init(catNum);
  
  pSecurity = new BLESecurity();
  pSecurity->setAuthenticationMode(ESP_LE_AUTH_REQ_SC_ONLY);
  pSecurity->setCapability(ESP_IO_CAP_OUT);
  pSecurity->setInitEncryptionKey(ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK);

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
