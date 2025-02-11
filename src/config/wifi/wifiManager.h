#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>

extern bool haveInternet;

class WiFiManager {
public:
    static void initWiFi();
    static void handleWiFi();
    static bool isInternetAvailable();
    static void reconnectWiFi();
    static void printWifiData();
    
private:
    static void startSmartConfig();
    static void checkInternetStatus();
};

#endif
