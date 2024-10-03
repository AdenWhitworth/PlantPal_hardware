#ifndef BLE_HANDLER_H
#define BLE_HANDLER_H

/**
 * @file BLEHandler.h
 * @brief Header file for Bluetooth Low Energy (BLE) handling.
 * 
 * This file contains declarations for functions and variables 
 * related to BLE functionality, including connection management 
 * and handling Wi-Fi credentials.
 */

/** 
 * @brief Flag indicating the connection status of a BLE device.
 * 
 * This variable is used to track whether a device is connected 
 * to the BLE server.
 */
extern bool deviceConnected;

/**
 * @brief Turns off Bluetooth Low Energy (BLE) and releases resources.
 * 
 * This function stops BLE advertising, disconnects any connected 
 * device, deinitializes BLE, and releases memory allocated for 
 * BLE security features.
 */
void turnOffBle();

/**
 * @brief Initializes and starts Bluetooth Low Energy (BLE) functionality.
 * 
 * This function sets up the BLE device, creates the BLE server and 
 * services, and starts advertising to allow connections from BLE clients.
 */
void beginBLE();

/**
 * @brief Handles new Wi-Fi credentials received via BLE.
 * 
 * If new credentials are received, this function will turn off BLE 
 * and attempt to connect to Wi-Fi with the new credentials.
 */
void handleNewCredentials();

#endif
