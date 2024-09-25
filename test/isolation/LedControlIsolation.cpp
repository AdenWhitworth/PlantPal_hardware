#include <Arduino.h>

#define red 26
#define green 32
#define blue 27

#define PWM1_Ch    0
#define PWM1_Res   8
#define PWM1_Freq  1000

#define PWM2_Ch    1
#define PWM2_Res   8
#define PWM2_Freq  1000

#define PWM3_Ch    2
#define PWM3_Res   8
#define PWM3_Freq  1000

void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("Begin");
  
  ledcAttachPin(red, PWM1_Ch);
  ledcSetup(PWM1_Ch, PWM1_Freq, PWM1_Res);  

  ledcAttachPin(green, PWM2_Ch);
  ledcSetup(PWM2_Ch, PWM2_Freq, PWM2_Res);

  ledcAttachPin(blue, PWM3_Ch);
  ledcSetup(PWM3_Ch, PWM3_Freq, PWM3_Res);
}

void loop() {

  Serial.println("Start Loop");
  
  setColor(255, 0, 0); // Red Color
  delay(1000);
  setColor(0, 255, 0); // Green Color
  delay(1000);
  setColor(0, 0, 255); // Blue Color
  delay(1000);
  setColor(255, 255, 255); // White Color
  delay(1000);
  setColor(170, 0, 255); // Purple Color
  delay(1000);
  setColor(256, 256, 256); // LED OFF
  
  Serial.println("End Loop");
  delay(4000);
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
