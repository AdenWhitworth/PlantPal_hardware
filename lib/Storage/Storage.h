#ifndef STORAGE_H
#define STORAGE_H
#include <Arduino.h>

void storeWifiCredentials(String &storedSSID, String &storedPassword);
void loadWifiCredentials(String &loadedSSID, String &loadedPassword);
void resetWifiPreferences();
void updateFirmwarePreferences();
void checkFirmware();

#endif