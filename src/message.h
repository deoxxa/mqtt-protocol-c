#ifndef MQTT_MESSAGE_H
#define MQTT_MESSAGE_H

#include <stdint.h>

#include "buffer.h"
#include "header.h"
#include "payload.h"

typedef struct mqtt_message_s {
  mqtt_header_t header;
  mqtt_payload_t payload;
} mqtt_message_t;

void mqtt_message_init(mqtt_message_t* message);
void mqtt_message_dump(mqtt_message_t* message);

#endif
