#ifndef MQTT_HANDLER_CONSTANTS_H
#define MQTT_HANDLER_CONSTANTS_H

namespace MqttSettings {
    const int MAX_RETRIES = 5;
    const int RETRY_DELAY_SECONDS = 1;
    const int RETRY_DELAY = RETRY_DELAY_SECONDS * 1000;
}

#endif