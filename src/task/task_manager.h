#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <Arduino.h>

void handleTasks();
void handlePeriodicTasks();
void readSensorsAndSendMQTT();
void printStoredEvents();

#endif
