#include <string.h>

#include "message.h"

#include "serialiser.h"

void mqtt_serialiser_init(mqtt_serialiser_t* serialiser) {
  memset(serialiser, 0, sizeof (mqtt_serialiser_t));
}

size_t mqtt_serialiser_size(mqtt_serialiser_t* serialiser, mqtt_message_t* message) {
  return 0;
}

int mqtt_serialiser_write(mqtt_serialiser_t* serialiser, mqtt_message_t* message, uint8_t* buffer, size_t len) {
  return 0;
}
