/**
 * @file LedControl.h
 * @brief Header file for controlling RGB LED behavior.
 * 
 * This header file contains the declarations of functions and 
 * external variables used for managing RGB LED operations, 
 * including initialization, color setting, fading effects, 
 * and blinking behavior.
 */
#ifndef LED_CONTROL_H
#define LED_CONTROL_H

/**
 * @brief Flag to indicate whether the LED should continue blinking.
 * 
 * This variable is marked as volatile since it may be accessed 
 * by both the main program and tasks running under FreeRTOS.
 */
extern volatile bool keepBlinking;

/**
 * @brief Handle for the LED blinking task.
 * 
 * This variable is used to manage the FreeRTOS task that handles 
 * the blinking of the RGB LED.
 */
extern TaskHandle_t ledTaskHandle;

/**
 * @brief Array to store the current RGB color values.
 * 
 * This array holds the intensity values for the current color of 
 * the RGB LED in the format [red, green, blue].
 */
extern int currentColor[3];

/**
 * @brief Initializes the RGB LED by attaching the LED pins to channels 
 *        and setting up PWM parameters.
 */
void initRgbLed();

/**
 * @brief Sets the color of the RGB LED based on the provided red, green, 
 *        and blue values.
 * 
 * @param redValue The intensity of the red color (0-255, 256 means off).
 * @param greenValue The intensity of the green color (0-255, 256 means off).
 * @param blueValue The intensity of the blue color (0-255, 256 means off).
 */
void setColor(int redValue, int greenValue, int blueValue);

/**
 * @brief Fades the RGB LED to the specified color over time.
 * 
 * @param color An array containing the target RGB color values.
 */
void fadeToColor(const int color[3]);

/**
 * @brief Fades the RGB LED in and out to the specified color while 
 *        keepBlinking is true.
 * 
 * @param color An array containing the RGB color values to fade.
 */
void fadeInAndOutColor(const int color[3]);

/**
 * @brief Task that handles fading the RGB LED based on the provided color.
 * 
 * @param pvParameters Pointer to the color values to fade to.
 */
void fadeLEDTask(void *pvParameters);

/**
 * @brief Begins blinking the RGB LED with the specified color.
 * 
 * @param color An array containing the RGB color values to blink.
 */
void beginBlinking(const int color[3]);

/**
 * @brief Ends the blinking behavior and fades the LED to the specified color.
 * 
 * @param color An array containing the RGB color values to fade to.
 */
void endBlinking(const int color[3]);

/**
 * @brief Sets the current color of the RGB LED.
 * 
 * @param color An array containing the RGB color values to set.
 */
void setCurrentColor(const int color[3]);

/**
 * @brief Switches the blinking color of the LED to a new color.
 * 
 * @param color An array containing the RGB color values to switch to.
 */
void switchBlinkingColor(const int color[3]);

/**
 * @brief Determines the assessment color based on soil sensor readings.
 * 
 * @return A pointer to the RGB color array representing the assessment result.
 */
const int* assessmentColor();

#endif 
