#ifndef MQTT_SERIALISER_H
#define MQTT_SERIALISER_H

#include "errors.h"
#include "message.h"

typedef enum mqtt_serialiser_rc_e {
  MQTT_SERIALISER_RC_ERROR,
  MQTT_SERIALISER_RC_SUCCESS,
} mqtt_serialiser_rc_t;

typedef struct mqtt_serialiser_s {
  mqtt_error_t error;
} mqtt_serialiser_t;

void mqtt_serialiser_init(mqtt_serialiser_t* serialiser);
size_t mqtt_serialiser_size(mqtt_serialiser_t* serialiser, mqtt_message_t* message);
mqtt_serialiser_rc_t mqtt_serialiser_write(mqtt_serialiser_t* serialiser, mqtt_message_t* message, uint8_t* buffer, size_t len);

#endif
