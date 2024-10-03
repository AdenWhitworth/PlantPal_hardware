#ifndef BLE_HANDLER_CONSTANTS_H
#define BLE_HANDLER_CONSTANTS_H

/**
 * @file BLEHandlerConstants.h
 * @brief Constants for Bluetooth Low Energy (BLE) settings.
 * 
 * This file contains constants that define delays and timeouts 
 * related to BLE operations, such as disconnection and response 
 * delays.
 */

namespace BleSettings {
    /**
     * @brief The delay time in seconds before disconnecting from a BLE device.
     * 
     * This constant defines the duration (in seconds) to wait 
     * before proceeding with the disconnection from a BLE device.
     */
    const int BLE_DISCONNECT_DELAY_SECONDS = 2;

    /**
     * @brief The delay time in milliseconds before disconnecting from a BLE device.
     * 
     * This constant converts the disconnection delay from seconds 
     * to milliseconds for use in BLE operations.
     */
    const int BLE_DISCONNECT_DELAY = BLE_DISCONNECT_DELAY_SECONDS * 1000;

    /**
     * @brief The delay time in seconds for BLE response.
     * 
     * This constant defines the duration (in seconds) to wait 
     * before sending a response after receiving BLE data.
     */
    const int BLE_RESPONSE_DELAY_SECONDS = 4;

    /**
     * @brief The delay time in milliseconds for BLE response.
     * 
     * This constant converts the response delay from seconds 
     * to milliseconds for use in BLE operations.
     */
    const int BLE_RESPONSE_DELAY = BLE_RESPONSE_DELAY_SECONDS * 1000; 
}

#endif