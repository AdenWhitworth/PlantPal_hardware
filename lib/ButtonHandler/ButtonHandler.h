#ifndef ButtonHandler_h
#define ButtonHandler_h

/**
 * @file ButtonHandler.h
 * @brief Header file for handling status button functionalities.
 * 
 * This header defines functions related to the status button 
 * on the ESP32 project. It provides the necessary declarations 
 * for setting up the button interrupt, handling button actions, 
 * and managing the button press event.
 */

#include <Arduino.h>

/**
 * @brief Handles actions based on the status button state.
 * 
 * If the status button has been pressed, this function checks 
 * the blinking state. It either stops blinking and turns off 
 * BLE or starts blinking and initializes BLE depending on the 
 * current state.
 */
void handleStatusButtonActions();

/**
 * @brief Sets up the interrupt for the status button.
 * 
 * Configures the status button pin as an input with pull-up 
 * resistor to detect button presses.
 */
void setupStatusButtonInterrupt();

/**
 * @brief Attaches the interrupt for the status button.
 * 
 * Configures the interrupt to trigger the statusBtnTriggered 
 * function on a falling edge (button press).
 */
void attachStatusButtonInterrupt();

/**
 * @brief Interrupt service routine for the status button.
 * 
 * This function is called when the status button is pressed. 
 * It checks the time since the last interrupt to debounce the 
 * button press and sets the statusButtonPressed flag accordingly.
 */
void IRAM_ATTR statusBtnTriggered();

#endif
