/*
#include <Preferences.h>
#include "AESLib.h"
#include "arduino_base64.hpp"

// Declare global objects
AESLib aesLib;
Preferences preferences;

// Function prototypes
void encryptAndStore(const char* ssid, const char* password);
void loadAndDecrypt(char* ssidBuffer, char* passwordBuffer);

// Function to encrypt and store SSID and password
void encryptAndStore(const char* ssid, const char* password) {
    // Convert SSID and password to Strings
    String ssidStr = String(ssid);
    String passwordStr = String(password);

    // Encrypt SSID
    String encryptedSSID = encrypt(ssidStr);
    // Encrypt Password
    String encryptedPassword = encrypt(passwordStr);

    Serial.println("Encrypted Values");
    Serial.println(encryptedSSID);
    Serial.println(encryptedPassword);

    // Store encrypted SSID and password in Preferences
    preferences.begin("wifi", false); // Open Preferences in write mode
    preferences.putString("ssid", encryptedSSID);
    preferences.putString("password", encryptedPassword);
    preferences.end();
}

// Function to load and decrypt SSID and password
void loadAndDecrypt(String &ssid, String &password) {
    preferences.begin("wifi", true); // Open Preferences in read mode
    String encryptedSSID = preferences.getString("ssid");
    String encryptedPassword = preferences.getString("password");
    preferences.end();

    // Decrypt the loaded SSID and password
    ssid = decrypt(encryptedSSID);
    password = decrypt(encryptedPassword);
}

// Main encryption function
String encrypt(String inputText) {
    int bytesInputLength = inputText.length() + 1;
    byte bytesInput[bytesInputLength];
    inputText.getBytes(bytesInput, bytesInputLength);
    int outputLength = aesLib.get_cipher_length(bytesInputLength);
    byte bytesEncrypted[outputLength];

    // Key and IV
    byte aesKey[] = { 23, 45, 56, 67, 67, 87, 98, 12, 32, 34, 45, 56, 67, 87, 65, 5 };
    byte aesIv[] = { 123, 43, 46, 89, 29, 187, 58, 213, 78, 50, 19, 106, 205, 1, 5, 7 };

    aesLib.set_paddingmode((paddingMode)0);
    aesLib.encrypt(bytesInput, bytesInputLength, bytesEncrypted, aesKey, 16, aesIv);

    char base64EncodedOutput[base64::encodeLength(outputLength)];
    base64::encode(bytesEncrypted, outputLength, base64EncodedOutput);

    return String(base64EncodedOutput);
}

// Main decryption function
String decrypt(String encryptedBase64Text) {
    int originalBytesLength = base64::decodeLength(encryptedBase64Text.c_str());
    byte encryptedBytes[originalBytesLength];
    byte decryptedBytes[originalBytesLength];

    base64::decode(encryptedBase64Text.c_str(), encryptedBytes);

    // Key and IV
    byte aesKey[] = { 23, 45, 56, 67, 67, 87, 98, 12, 32, 34, 45, 56, 67, 87, 65, 5 };
    byte aesIv[] = { 123, 43, 46, 89, 29, 187, 58, 213, 78, 50, 19, 106, 205, 1, 5, 7 };

    aesLib.set_paddingmode((paddingMode)0);
    aesLib.decrypt(encryptedBytes, originalBytesLength, decryptedBytes, aesKey, 16, aesIv);

    return String((char*)decryptedBytes);
}

void setup() {
    Serial.begin(115200);
    
    delay(2000);

    Serial.println("--- Device Started ---");

    // Example SSID and Password
    const char* ssid = "YourSSID";
    const char* password = "YourPassword";

    // Encrypt and store the SSID and password
    encryptAndStore(ssid, password);

    // Variables to hold loaded SSID and password
    String loadedSSID;
    String loadedPassword;

    // Load and decrypt the SSID and password
    loadAndDecrypt(loadedSSID, loadedPassword);

    // Print the loaded SSID and password
    Serial.print("Loaded SSID: ");
    Serial.println(loadedSSID);
    Serial.print("Loaded Password: ");
    Serial.println(loadedPassword);

    ssid = loadedSSID.c_str(); // Use this if you want to update the ssid variable
    password = loadedPassword.c_str();

    // Redeclare
    Serial.print("Redeclare SSID: ");
    Serial.println(ssid);
    Serial.print("Redeclare Password: ");
    Serial.println(password);

    Serial.println();
    Serial.println("--- The End ---");
}

void loop() {
    // Your main code here
}
*/
