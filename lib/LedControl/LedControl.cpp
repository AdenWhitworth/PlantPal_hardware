#include <Arduino.h>
#include "LedControl.h"
#include "LedControlConstants.h"
#include "../../include/PinConfig.h"

bool keepBlinking = false;

void setColor(int redValue, int greenValue, int blueValue){

  static bool isFirstCall = true;

  if (isFirstCall){
    ledcAttachPin(RedLedPin, PWM1_Ch);
    ledcSetup(PWM1_Ch, PWM1_Freq, PWM1_Res);
    
    ledcAttachPin(GreenLedPin, PWM2_Ch);
    ledcSetup(PWM2_Ch, PWM2_Freq, PWM2_Res);
    
    ledcAttachPin(BlueLedPin, PWM3_Ch);
    ledcSetup(PWM3_Ch, PWM3_Freq, PWM3_Res);
    isFirstCall = false;
  }
  
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

void fadeToColor(const int color[3], int fadeDuration) {
  for (int i = 0; i <= 255; i++) {
    setColor(color[0] * i / 255, color[1] * i / 255, color[2] * i / 255);
    delay(fadeDuration);
  }
}

void fadeInAndOutColor(const int color[3], int fadeDuration) {
  while (keepBlinking) {
    for (int i = 0; i <= 255; i++) {
      if (!keepBlinking) return;
      setColor(color[0] * i / 255, color[1] * i / 255, color[2] * i / 255);
      delay(fadeDuration);
    }
    
    for (int i = 255; i >= 0; i--) {
      if (!keepBlinking) return; 
      setColor(color[0] * i / 255, color[1] * i / 255, color[2] * i / 255);
      delay(fadeDuration);
    }
  }
}
