#include <Arduino.h>
#include "LedControl.h"
#include "LedControlConstants.h"
#include "SoilSensor.h"
#include "../../include/PinConfig.h"

volatile bool keepBlinking = false;

int currentColor[3] = {0, 0, 0};

TaskHandle_t ledTaskHandle;

void initRgbLed(){
  ledcAttachPin(RedLedPin, PWM1_Ch);
  ledcSetup(PWM1_Ch, PWM1_Freq, PWM1_Res);
  
  ledcAttachPin(GreenLedPin, PWM2_Ch);
  ledcSetup(PWM2_Ch, PWM2_Freq, PWM2_Res);
  
  ledcAttachPin(BlueLedPin, PWM3_Ch);
  ledcSetup(PWM3_Ch, PWM3_Freq, PWM3_Res);
  setColor(256, 256, 256);
}

void setColor(int redValue, int greenValue, int blueValue){
  
  if (redValue != 256){
    redValue = 255 - redValue;
  }

  if (greenValue != 256){
    greenValue = 255 - greenValue;
  }

  if (blueValue != 256){
    blueValue = 255 - blueValue;
  }
  
  ledcWrite(0,redValue); 
  ledcWrite(1,greenValue);
  ledcWrite(2,blueValue);
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