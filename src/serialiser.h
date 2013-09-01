#ifndef MQTT_SERIALISER_H
#define MQTT_SERIALISER_H

typedef struct mqtt_serialiser_s {
  void* user;
} mqtt_serialiser_t;

void mqtt_serialiser_init(mqtt_serialiser_t* serialiser);

#endif
