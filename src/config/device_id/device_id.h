#ifndef DEVICE_ID_H
#define DEVICE_ID_H

#include <Arduino.h>

class DeviceID {
public:
    static String getUniqueID();
};

#endif
