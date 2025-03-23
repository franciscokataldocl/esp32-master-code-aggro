#include "wifiLedControl.h"
#include "config/config.h"




void WifiLedControl::turnOnLed(bool turnOn) {
    pinMode(WIFI_LED_RED_PIN, OUTPUT);
    if(turnOn == 1){
        digitalWrite(WIFI_LED_RED_PIN, HIGH); 
    } else{
        digitalWrite(WIFI_LED_RED_PIN, LOW); 
    }

    
}
