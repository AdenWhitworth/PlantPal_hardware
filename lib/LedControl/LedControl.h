#ifndef LED_CONTROL_H
#define LED_CONTROL_H

extern bool keepBlinking;
void setColor(int redValue, int greenValue, int blueValue);
void fadeToColor(const int color[3], int fadeDuration);
void fadeInAndOutColor(const int color[3], int fadeDuration);

#endif
