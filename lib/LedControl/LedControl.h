#ifndef LED_CONTROL_H
#define LED_CONTROL_H

extern volatile bool keepBlinking;
extern TaskHandle_t ledTaskHandle;
extern int currentColor[3];

void initRgbLed();
void setColor(int redValue, int greenValue, int blueValue);
void fadeToColor(const int color[3]);
void fadeInAndOutColor(const int color[3]);
void fadeLEDTask(void *pvParameters);
void beginBlinking(const int color[3]);
void endBlinking(const int color[3]);
void setCurrentColor(const int color[3]);

#endif
