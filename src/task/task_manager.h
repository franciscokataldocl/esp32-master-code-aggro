#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <Arduino.h>

extern unsigned long bootTime;  // 👈 Para que `task_manager.cpp` vea esta variable

void handleTasks();
void handlePeriodicTasks();
void readSensorsAndSendMQTT();
void printStoredEvents();




#endif
