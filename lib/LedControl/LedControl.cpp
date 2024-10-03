/**
 * @file LedControl.cpp
 * @brief Control and management of RGB LED behavior for the ESP32 project.
 * 
 * This file contains functions for initializing and controlling 
 * RGB LEDs, including setting colors, fading effects, and 
 * managing blinking behavior. The RGB LED is controlled using 
 * PWM (Pulse Width Modulation) and tasks are managed with FreeRTOS.
 */
#include <Arduino.h>
#include "LedControl.h"
#include "LedControlConstants.h"
#include "SoilSensor.h"
#include "../../include/PinConfig.h"

volatile bool keepBlinking = false;

int currentColor[3] = {0, 0, 0};

TaskHandle_t ledTaskHandle;

/**
 * @brief Initializes the RGB LED by attaching the LED pins to channels 
 *        and setting up PWM parameters.
 */
void initRgbLed(){
  ledcAttachPin(RedLedPin, RED_CHANNEL);
  ledcSetup(RED_CHANNEL, RgbLedSettings::PWM_FREQUENCY, RgbLedSettings::PWM_RESOLUTION);
  
  ledcAttachPin(GreenLedPin, GREEN_CHANNEL);
  ledcSetup(GREEN_CHANNEL, RgbLedSettings::PWM_FREQUENCY, RgbLedSettings::PWM_RESOLUTION);
  
  ledcAttachPin(BlueLedPin, BLUE_CHANNEL);
  ledcSetup(BLUE_CHANNEL, RgbLedSettings::PWM_FREQUENCY, RgbLedSettings::PWM_RESOLUTION);
  setColor(256, 256, 256);
}

/**
 * @brief Sets the color of the RGB LED based on the provided red, green, 
 *        and blue values.
 * 
 * @param redValue The intensity of the red color (0-255, 256 means off).
 * @param greenValue The intensity of the green color (0-255, 256 means off).
 * @param blueValue The intensity of the blue color (0-255, 256 means off).
 */
void setColor(int redValue, int greenValue, int blueValue){
  
  redValue = (redValue != 256) ? 255 - redValue : 256;
  greenValue = (greenValue != 256) ? 255 - greenValue : 256;
  blueValue = (blueValue != 256) ? 255 - blueValue : 256;
  
  ledcWrite(RED_CHANNEL, redValue);
  ledcWrite(GREEN_CHANNEL, greenValue);
  ledcWrite(BLUE_CHANNEL, blueValue);
}

/**
 * @brief Fades the RGB LED to the specified color over time.
 * 
 * @param color An array containing the target RGB color values.
 */
void fadeToColor(const int color[3]) {
  setCurrentColor(color);
  for (int i = 0; i <= 255; i++) {
    setColor(color[0] * i / 255, color[1] * i / 255, color[2] * i / 255);
    vTaskDelay(pdMS_TO_TICKS(ColorSettings::FADEDURATION));
  }
}

/**
 * @brief Fades the RGB LED in and out to the specified color while 
 *        keepBlinking is true.
 * 
 * @param color An array containing the RGB color values to fade.
 */
void fadeInAndOutColor(const int color[3]) {
  setCurrentColor(color);
  while (keepBlinking) {
    for (int i = 0; i <= 255; i++) {
      if (!keepBlinking) return;
      setColor(color[0] * i / 255, color[1] * i / 255, color[2] * i / 255);
      vTaskDelay(pdMS_TO_TICKS(ColorSettings::FADEDURATION));
    }
    
    for (int i = 0; i <= 255; i++) {
      if (!keepBlinking) return;
      int iReversed = 255 - i ; 
      setColor(color[0] * iReversed / 255, color[1] * iReversed / 255, color[2] * iReversed / 255);
      vTaskDelay(pdMS_TO_TICKS(ColorSettings::FADEDURATION));
    }
  }
}

/**
 * @brief Task that handles fading the RGB LED based on the provided color.
 * 
 * @param pvParameters Pointer to the color values to fade to.
 */
void fadeLEDTask(void *pvParameters) {
  const int* color = (const int*)pvParameters;
  setCurrentColor(color);
  
  while (keepBlinking) {
    for (int i = 0; i <= 255; i++) {
      if (!keepBlinking) {
        return;
      }
      setColor(color[0] * i / 255, color[1] * i / 255, color[2] * i / 255);
      vTaskDelay(pdMS_TO_TICKS(ColorSettings::FADEDURATION));
    }
    
    for (int i = 0; i <= 255; i++) {
      if (!keepBlinking) {
        return;
      }
      int iReversed = 255 - i; 
      setColor(color[0] * iReversed / 255, color[1] * iReversed / 255, color[2] * iReversed / 255);
      vTaskDelay(pdMS_TO_TICKS(ColorSettings::FADEDURATION));
    }
  }
}

/**
 * @brief Begins blinking the RGB LED with the specified color.
 * 
 * @param color An array containing the RGB color values to blink.
 */
void beginBlinking(const int color[3]){
  keepBlinking = true;
  
  if (xTaskCreate(fadeLEDTask, "FadeLEDTask", 4096, (void*)color, 1, &ledTaskHandle) != pdPASS) {
    Serial.println("Task creation failed!");
  }
}

/**
 * @brief Ends the blinking behavior and fades the LED to the specified color.
 * 
 * @param color An array containing the RGB color values to fade to.
 */
void endBlinking(const int color[3]){
  keepBlinking = false;

  if (ledTaskHandle != NULL) {
    vTaskDelete(ledTaskHandle);
    ledTaskHandle = NULL;
  }

  fadeToColor(color);
}

/**
 * @brief Switches the blinking color of the LED to a new color.
 * 
 * @param color An array containing the RGB color values to switch to.
 */
void switchBlinkingColor(const int color[3]){
  if (ledTaskHandle != NULL) {
    vTaskDelete(ledTaskHandle);
    ledTaskHandle = NULL;
  }

  beginBlinking(color);
}

/**
 * @brief Sets the current color of the RGB LED.
 * 
 * @param color An array containing the RGB color values to set.
 */
void setCurrentColor(const int color[3]) {
  currentColor[0] = color[0];
  currentColor[1] = color[1];
  currentColor[2] = color[2];
}

/**
 * @brief Determines the assessment color based on soil sensor readings.
 * 
 * @return A pointer to the RGB color array representing the assessment result.
 */
const int* assessmentColor(){
  if (assessSoil()){
    return ColorSettings::RED;
  } else {
    return ColorSettings::GREEN;
  }
  
}