/**
 * @file LedControlConstants.h
 * @brief Header file for RGB LED control constants and settings.
 * 
 * This header file contains enumerations and constant values used for 
 * configuring RGB LED channels and color settings.
 */

#ifndef LED_CONTROL_CONSTANTS_H
#define LED_CONTROL_CONSTANTS_H

/**
 * @enum ColorChannels
 * @brief Enumeration for the RGB LED color channels.
 * 
 * This enumeration defines the channel indices for the red, green, 
 * and blue color components of the RGB LED.
 */
enum ColorChannels {
    RED_CHANNEL = 0,    // Index for the red color channel
    GREEN_CHANNEL = 1,  // Index for the green color channel
    BLUE_CHANNEL = 2    // Index for the blue color channel
};

/**
 * @namespace RgbLedSettings
 * @brief Settings for RGB LED control.
 * 
 * This namespace contains constant values for configuring the 
 * PWM (Pulse Width Modulation) resolution and frequency for the 
 * RGB LED.
 */
namespace RgbLedSettings {
    const int PWM_RESOLUTION = 8;  // PWM resolution for the LED
    const int PWM_FREQUENCY = 1000; // PWM frequency for the LED in Hz
}

/**
 * @namespace ColorSettings
 * @brief Predefined RGB color values and fade duration.
 * 
 * This namespace contains constant arrays representing common 
 * RGB color values and a constant for the fade duration.
 */
namespace ColorSettings {
    const int GREEN[3] = {0, 255, 0};       // RGB representation of green
    const int RED[3] = {255, 0, 0};         // RGB representation of red
    const int BLUE[3] = {0, 0, 255};        // RGB representation of blue
    const int WHITE[3] = {255, 255, 255};   // RGB representation of white
    const int OFF[3] = {256, 256, 256};     // RGB representation of off (not lit)
    const int ORANGE[3] = {255, 165, 0};    // RGB representation of orange
    const int FADEDURATION = 5;               // Duration for fade effect in milliseconds
}

#endif