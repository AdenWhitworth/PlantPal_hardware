#ifndef SOILASSESSMENT_H
#define SOILASSESSMENT_H

#include "SoilSensor.h"

void scheduledSoilAssessment(unsigned long currentMillis);
void scheduledAutoWaterAssessment(unsigned long currentMillis);
void manualWaterAssessment();

#endif // SOILASSESSMENT_H
