#include <Arduino.h>
#include "setup/setup_helpers.h"
#include "task/task_manager.h"

void setup() {
    Serial.begin(115200);
    initSystem();
}

void loop() {
    handleTasks();
}
