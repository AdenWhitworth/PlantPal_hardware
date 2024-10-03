#ifndef PINCONFIG_H
#define PINCONFIG_H

/**
 * @file PinConfig.h
 * @brief Configuration for pin assignments on the ESP32.
 *
 * This header file contains the pin assignments for controlling various
 * hardware components including pumps, RGB LEDs, and buttons.
 */

/** 
 * @brief Pin for controlling the pump.
 * 
 * This constant defines the GPIO pin connected to the pump control circuit.
 * It is used to turn the pump on or off.
 */
const int pumpControlPin = 12;

/** 
 * @brief Pin assignments for the RGB LED.
 * 
 * These constants define the GPIO pins connected to the individual color 
 * channels of the RGB LED.
 */
const byte GreenLedPin = 32; ///< GPIO pin for the green LED channel
const byte RedLedPin = 26;   ///< GPIO pin for the red LED channel
const byte BlueLedPin = 27;  ///< GPIO pin for the blue LED channel

/** 
 * @brief Pin for the status button.
 * 
 * This constant defines the GPIO pin connected to the status button. It 
 * is used to trigger actions based on button presses.
 */
#define statusBTNPin  4

#endif