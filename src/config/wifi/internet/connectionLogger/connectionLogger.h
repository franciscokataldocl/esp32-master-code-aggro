#ifndef CONNECTION_LOGGER_H
#define CONNECTION_LOGGER_H

#include <Arduino.h>
#include "../../../../common/microsd/microSd.h"
#include "config.h"

class ConnectionLogger {
  public:
    ConnectionLogger(MicroSD& sd);
    void logEvent(const String& status);
    String getCurrentTimestamp();

  private:
    MicroSD& _sd;
    const char* filename = EVENTS_LOG_PATH;
};

#endif
