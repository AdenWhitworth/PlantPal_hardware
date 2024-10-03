#include <Arduino.h>
#include "LedControl.h"
#include "LedControlConstants.h"
#include "SoilSensor.h"
#include "../../include/PinConfig.h"

volatile bool keepBlinking = false;

int currentColor[3] = {0, 0, 0};

TaskHandle_t ledTaskHandle;

void initRgbLed(){
  ledcAttachPin(RedLedPin, RED_CHANNEL);
  ledcSetup(RED_CHANNEL, RgbLedSettings::PWM_FREQUENCY, RgbLedSettings::PWM_RESOLUTION);
  
  ledcAttachPin(GreenLedPin, GREEN_CHANNEL);
  ledcSetup(GREEN_CHANNEL, RgbLedSettings::PWM_FREQUENCY, RgbLedSettings::PWM_RESOLUTION);
  
  ledcAttachPin(BlueLedPin, BLUE_CHANNEL);
  ledcSetup(BLUE_CHANNEL, RgbLedSettings::PWM_FREQUENCY, RgbLedSettings::PWM_RESOLUTION);
  setColor(256, 256, 256);
}

void setColor(int redValue, int greenValue, int blueValue){
  
  redValue = (redValue != 256) ? 255 - redValue : 256;
  greenValue = (greenValue != 256) ? 255 - greenValue : 256;
  blueValue = (blueValue != 256) ? 255 - blueValue : 256;
  
  ledcWrite(RED_CHANNEL, redValue);
  ledcWrite(GREEN_CHANNEL, greenValue);
  ledcWrite(BLUE_CHANNEL, blueValue);
}

void fadeToColor(const int color[3]) {
  setCurrentColor(color);
  for (int i = 0; i <= 255; i++) {
    setColor(color[0] * i / 255, color[1] * i / 255, color[2] * i / 255);
    vTaskDelay(pdMS_TO_TICKS(ColorSettings::FADEDURATION));
  }
}

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

void beginBlinking(const int color[3]){
  keepBlinking = true;
  
  if (xTaskCreate(fadeLEDTask, "FadeLEDTask", 4096, (void*)color, 1, &ledTaskHandle) != pdPASS) {
    Serial.println("Task creation failed!");
  }
}

void endBlinking(const int color[3]){
  keepBlinking = false;

  if (ledTaskHandle != NULL) {
    vTaskDelete(ledTaskHandle);
    ledTaskHandle = NULL;
  }

  fadeToColor(color);
}

void switchBlinkingColor(const int color[3]){
  if (ledTaskHandle != NULL) {
    vTaskDelete(ledTaskHandle);
    ledTaskHandle = NULL;
  }

  beginBlinking(color);
}

void setCurrentColor(const int color[3]) {
  currentColor[0] = color[0];
  currentColor[1] = color[1];
  currentColor[2] = color[2];
}

const int* assessmentColor(){
  if (assessSoil()){
    return ColorSettings::RED;
  } else {
    return ColorSettings::GREEN;
  }
  
}