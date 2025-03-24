#ifndef SETUP_HELPERS_H
#define SETUP_HELPERS_H

#include <Arduino.h>

void initSystem();
void initSensorColorCalibration();
void initTime();

extern unsigned long bootTime;


#endif
