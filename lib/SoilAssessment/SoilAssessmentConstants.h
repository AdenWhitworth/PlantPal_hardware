#ifndef SOIL_ASSESSMENT_CONSTANTS_H
#define SOIL_ASSESSMENT_CONSTANTS_H

namespace AssessmentSettings {
    const unsigned long LOG_INTERVAL_HOURS = 4;
    const unsigned long AUTO_INTERVAL_HOURS = 1;
    const unsigned long LOG_INTERVAL = LOG_INTERVAL_HOURS * 60 * 60 * 1000;
    const unsigned long AUTO_INTERVAL = AUTO_INTERVAL_HOURS * 60 * 60 * 1000;
}

#endif