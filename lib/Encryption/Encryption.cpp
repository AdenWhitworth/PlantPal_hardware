#include "Encryption.h"
#include <AESLib.h>
#include <Preferences.h>
#include "WifiHandler.h"
#include "../../src/utilities.h"
#include <SPI.h>

AESLib aesLib;
Preferences preferences;

void encryptAndStore(const char* ssid, const char* password) {
    int ssidLen = strlen(ssid);
    int passwordLen = strlen(password);

    byte encryptedSSID[64];
    byte encryptedPassword[128];

    aesLib.encrypt((byte*)ssid, ssidLen, encryptedSSID, aesKey, sizeof(aesKey), aesIV);

    aesLib.encrypt((byte*)password, passwordLen, encryptedPassword, aesKey, sizeof(aesKey), aesIV);

    preferences.begin("wifi", false);
    preferences.putBytes("ssid", encryptedSSID, sizeof(encryptedSSID));
    preferences.putBytes("password", encryptedPassword, sizeof(encryptedPassword));
    preferences.end();
}

void loadAndDecrypt() {
    byte encryptedSSID[64];
    byte encryptedPassword[128];

    preferences.begin("wifi", true);
    size_t ssidLength = preferences.getBytes("ssid", encryptedSSID, sizeof(encryptedSSID));
    size_t passwordLength = preferences.getBytes("password", encryptedPassword, sizeof(encryptedPassword));
    preferences.end();

    char ssidBuffer[32];
    char passwordBuffer[64];
    
    aesLib.decrypt(encryptedSSID, ssidLength, (byte*)ssidBuffer, aesKey, sizeof(aesKey), aesIV);
    aesLib.decrypt(encryptedPassword, passwordLength, (byte*)passwordBuffer, aesKey, sizeof(aesKey), aesIV);

    ssid = strdup(ssidBuffer);
    password = strdup(passwordBuffer);
}
