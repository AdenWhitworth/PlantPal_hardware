/**
 * @file WifiHandler.h
 * @brief Header file for WiFi handling functions and variables.
 *
 * This file contains declarations for functions related to WiFi connection
 * management, including connecting to WiFi, checking the connection status, 
 * and handling disconnections. It also declares the SSID and password 
 * variables used for WiFi connectivity.
 */

#ifndef WIFI_HANDLER_H
#define WIFI_HANDLER_H

extern String ssid;      // The SSID of the WiFi network.
extern String password;  // The password for the WiFi network.

/**
 * @brief Connects to a specified WiFi network using the global SSID and password.
 *
 * This function initiates the connection process to the WiFi network defined 
 * by the global `ssid` and `password` variables. It logs the connection 
 * status and updates the connection state.
 */
void connectToWiFi();

/**
 * @brief Checks if the device is currently connected to WiFi.
 *
 * @return True if the device is connected to WiFi, false otherwise.
 */
bool checkWifiStatus();

/**
 * @brief Handles disconnection from the WiFi network and MQTT.
 *
 * This function attempts to disconnect from the WiFi network and MQTT broker 
 * if they are currently connected. It logs the disconnection attempt and 
 * status.
 *
 * @return True if disconnection from WiFi was successful, false otherwise.
 */
bool handleDisconnections();

#endif