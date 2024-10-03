/**
 * @file BLEHandlerConstants.h
 * @brief Constants for Bluetooth Low Energy (BLE) settings.
 * 
 * This file contains constants that define delays and timeouts 
 * related to BLE operations, such as disconnection and response 
 * delays.
 */
#ifndef BLE_HANDLER_CONSTANTS_H
#define BLE_HANDLER_CONSTANTS_H

/**
 * @brief Namespace containing settings for Bluetooth Low Energy (BLE).
 * 
 * The BleSettings namespace holds constants that control 
 * various timing aspects of BLE operations, such as 
 * disconnection and response delays.
 */
namespace BleSettings {
    const int BLE_DISCONNECT_DELAY_SECONDS = 2; // The delay time in seconds before disconnecting from a BLE device.
    const int BLE_DISCONNECT_DELAY = BLE_DISCONNECT_DELAY_SECONDS * 1000; // The delay time in milliseconds before disconnecting from a BLE device.
    const int BLE_RESPONSE_DELAY_SECONDS = 4; // The delay time in seconds for BLE response.
    const int BLE_RESPONSE_DELAY = BLE_RESPONSE_DELAY_SECONDS * 1000; // The delay time in milliseconds for BLE response.
}

#endif