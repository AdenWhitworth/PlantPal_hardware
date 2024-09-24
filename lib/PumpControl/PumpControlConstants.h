#ifndef PUMP_CONTROL_CONSTANTS_H
#define PUMP_CONTROL_CONSTANTS_H

namespace PumpSettings {
    const int MAX_PUMP_SPEED = 80; // Max set speed of the pump motor
    const int RAMP_DELAY = 5; // Delay in ms for pump to speed up to max speed
    const int PUMP_DURATION = 1000; // Duration in ms to hold pump at max speed
}

#endif