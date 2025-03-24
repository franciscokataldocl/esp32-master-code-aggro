#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>
#include "internet/connectionLogger/connectionLogger.h" // ✅ incluye esto

extern bool haveInternet;

class WiFiManager {
public:
    static void initWiFi(ConnectionLogger &logger); // Añade logger aquí también
    static void handleWiFi();
    static bool isInternetAvailable();
    static void reconnectWiFi();
    static void printWifiData();
    static void handleInternetCheck(ConnectionLogger &logger);
    
private:
    static void startSmartConfig(ConnectionLogger &logger);
    static void checkInternetStatus();
};

#endif
