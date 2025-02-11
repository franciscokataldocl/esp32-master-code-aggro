#include "wifiLedControl.h"


const int redLedPin = 23; 

void WifiLedControl::turnOnLed(bool turnOn) {
    pinMode(redLedPin, OUTPUT);
    if(turnOn == 1){
        digitalWrite(redLedPin, HIGH); 
    } else{
        digitalWrite(redLedPin, LOW); 
    }

    
}
