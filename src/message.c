#include <string.h>

#include "header.h"
#include "payload.h"

#include "message.h"

void mqtt_message_init(mqtt_message_t* message) {
  mqtt_header_init(&(message->header));
  mqtt_payload_init(&(message->payload));
}

void mqtt_message_dump(mqtt_message_t* message) {
  mqtt_header_dump(&(message->header));
  mqtt_payload_dump(&(message->payload));
}
