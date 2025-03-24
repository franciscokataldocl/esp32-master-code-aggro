#include <Arduino.h>
#include "setup/setup_helpers.h"
#include "task/task_manager.h"

unsigned long bootTime;  // 👈 GLOBAL para comparar en tasks



void setup() {
    Serial.begin(115200);
    bootTime = millis();         // 👈 ¡esta línea es clave!

    initSystem();
}

void loop() {
    handleTasks();
}
