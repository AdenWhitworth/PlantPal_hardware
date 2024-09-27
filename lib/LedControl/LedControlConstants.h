#ifndef LED_CONTROL_CONSTANTS_H
#define LED_CONTROL_CONSTANTS_H

#define PWM1_Ch    0
#define PWM1_Res   8
#define PWM1_Freq  1000
#define PWM2_Ch    1
#define PWM2_Res   8
#define PWM2_Freq  1000
#define PWM3_Ch    2
#define PWM3_Res   8
#define PWM3_Freq  1000

namespace ColorSettings {
    const int GREEN[3] = {0, 255, 0};
    const int RED[3] = {255, 0, 0};
    const int BLUE[3] = {0, 0, 255};
    const int WHITE[3] = {255, 255, 255};
    const int OFF[3] = { 256, 256, 256 };
    const int FADEDURATION = 5;
}

#endif