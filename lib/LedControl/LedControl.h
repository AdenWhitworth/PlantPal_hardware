#ifndef LED_CONTROL_H
#define LED_CONTROL_H

extern bool keepBlinking;
void setColor(int redValue, int greenValue, int blueValue);
void fadeToColor(int red, int green, int blue, int fadeDuration);
void fadeInAndOutColor(int red, int green, int blue, int fadeDuration);

#endif
