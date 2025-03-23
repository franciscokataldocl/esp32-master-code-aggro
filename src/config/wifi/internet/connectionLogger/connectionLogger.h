#ifndef CONNECTION_LOGGER_H
#define CONNECTION_LOGGER_H

#include <Arduino.h>
#include "../../../../common/microsd/microSd.h"

class ConnectionLogger {
  public:
    ConnectionLogger(MicroSD& sd);
    void logEvent(const String& status);
    String getCurrentTimestamp();

  private:
    MicroSD& _sd;
    const char* filename = "/events.json";
};

#endif
