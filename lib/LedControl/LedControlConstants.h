#ifndef LED_CONTROL_CONSTANTS_H
#define LED_CONTROL_CONSTANTS_H

enum ColorChannels {
    RED_CHANNEL = 0,
    GREEN_CHANNEL = 1,
    BLUE_CHANNEL = 2
};

namespace RgbLedSettings{
    const int PWM_RESOLUTION = 8;
    const int PWM_FREQUENCY = 1000;
}

namespace ColorSettings {
    const int GREEN[3] = {0, 255, 0};
    const int RED[3] = {255, 0, 0};
    const int BLUE[3] = {0, 0, 255};
    const int WHITE[3] = {255, 255, 255};
    const int OFF[3] = { 256, 256, 256 };
    const int ORANGE[3] = {255, 165, 0};
    const int FADEDURATION = 5;
}

#endif