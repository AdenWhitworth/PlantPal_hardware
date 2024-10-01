#ifndef WIFI_HANDLER_H
#define WIFI_HANDLER_H

extern String ssid;      
extern String password; 
void connectToWiFi();
bool checkWifiStatus();
void checkDisconnections();

#endif
