/**
 * @file ButtonHandlerConstants.h
 * @brief Constants for button handling in the ESP32 project.
 * 
 * This header file defines constants related to the 
 * handling of button inputs, specifically for debouncing 
 * delays to ensure reliable button presses are detected.
 */
#ifndef BUTTON_HANDLER_CONSTANTS_H
#define BUTTON_HANDLER_CONSTANTS_H

/**
 * @brief Namespace containing settings for button behavior.
 * 
 * The ButtonSettings namespace holds constants that control
 * the behavior of the button input, such as debounce timing.
 */
namespace ButtonSettings {
    const int DEBOUNCE_DELAY_MILLISECONDS = 200; // The debounce delay in milliseconds to filter out mechanical switch noise.
}

#endif