#ifndef PUMP_CONTROL_CONSTANTS_H
#define PUMP_CONTROL_CONSTANTS_H

namespace PumpSettings {
    const int PUMP_DURATION_SECONDS = 3;
    const int PUMP_DURATION = PUMP_DURATION_SECONDS * 1000;
    const int MAX_RETRY_ATTEMPTS = 4;
    const int RETRY_DELAY_SECONDS = 2;
    const int RETRY_DELAY = RETRY_DELAY_SECONDS * 1000;
}

#endif