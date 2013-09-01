#ifndef MQTT_SERIALISER_H
#define MQTT_SERIALISER_H

#include "message.h"

typedef struct mqtt_serialiser_s {
  void* user;
} mqtt_serialiser_t;

void mqtt_serialiser_init(mqtt_serialiser_t* serialiser);
void mqtt_serialiser_size(mqtt_serialiser_t* serialiser, mqtt_message_t* message);

#endif
