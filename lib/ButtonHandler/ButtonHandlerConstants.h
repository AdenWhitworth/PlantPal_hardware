#ifndef BUTTON_HANDLER_CONSTANTS_H
#define BUTTON_HANDLER_CONSTANTS_H

/**
 * @file ButtonHandlerConstants.h
 * @brief Constants for button handling in the ESP32 project.
 * 
 * This header file defines constants related to the 
 * handling of button inputs, specifically for debouncing 
 * delays to ensure reliable button presses are detected.
 */

namespace ButtonSettings {
    /** 
     * @brief The debounce delay in milliseconds.
     * 
     * This constant defines the duration to wait after a 
     * button press before considering it valid, helping to 
     * filter out noise from the mechanical switch. The value 
     * is set to 200 milliseconds.
     */
    const int DEBOUNCE_DELAY_MILLISECONDS = 200;
}

#endif