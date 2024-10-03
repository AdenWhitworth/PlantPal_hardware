/**
 * @file BleHandler.cpp
 * @brief Handles Bluetooth Low Energy (BLE) functionalities for the ESP32.
 * 
 * This file manages BLE server operations, connection handling, and 
 * parsing of Wi-Fi credentials received via BLE. It initializes 
 * BLE services, characteristics, and handles responses to connected devices.
 */
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

/**
 * @class MyServerCallbacks
 * @brief Callback class for BLE server events.
 * 
 * This class handles BLE server connection and disconnection events.
 */
class MyServerCallbacks : public BLEServerCallbacks {
  /**
   * @brief Called when a device connects to the BLE server.
   * 
   * @param pServer Pointer to the BLE server instance.
   */
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
    Serial.println("Device connected");
  }

  /**
   * @brief Called when a device disconnects from the BLE server.
   * 
   * @param pServer Pointer to the BLE server instance.
   */
  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
    Serial.println("Device disconnected");
  }
};

/**
 * @brief Turns off BLE and releases associated resources.
 * 
 * This function stops BLE advertising, disconnects the connected 
 * device (if any), deinitializes BLE, and deletes the BLESecurity 
 * instance if it was created.
 */
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

/**
 * @brief Parses the received Wi-Fi credentials from a string.
 * 
 * @param valueString The string containing the Wi-Fi credentials.
 * @param ssid Reference to a String to store the SSID.
 * @param password Reference to a String to store the password.
 * @return true if parsing was successful, false otherwise.
 */
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

/**
 * @class MyCallbacks
 * @brief Callback class for BLE characteristic events.
 * 
 * This class handles write events to the BLE characteristic,
 * including parsing Wi-Fi credentials and storing them.
 */
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

/**
 * @brief Handles the new Wi-Fi credentials received via BLE.
 * 
 * If new credentials are received, this function will turn off BLE, 
 * wait for a short period, and then attempt to connect to Wi-Fi.
 */
void handleNewCredentials() {
  if (recievedNewCredentials){
    turnOffBle();

    vTaskDelay(pdMS_TO_TICKS(BleSettings::BLE_RESPONSE_DELAY));

    connectToWiFi();
    recievedNewCredentials = false;
  }
}

/**
 * @brief Initializes BLE functionality.
 * 
 * This function sets up the BLE device, initializes security features,
 * creates the server and characteristic, and starts advertising.
 */
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
