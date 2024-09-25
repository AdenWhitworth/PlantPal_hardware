#ifndef ButtonHandler_h
#define ButtonHandler_h

#include <Arduino.h>

void setupStatusButtonInterrupt();
void attachStatusButtonInterrupt();
void IRAM_ATTR statusBtnTriggered();

#endif
