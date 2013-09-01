#include <stdio.h>

#include "header.h"

#include "payload.h"

void mqtt_payload_init(mqtt_payload_t* payload) {
}

void mqtt_payload_dump(mqtt_payload_t* payload) {
  printf("payload\n");
  printf("  type: %d\n", payload->type);

  if (payload->type == MQTT_MESSAGE_TYPE_CONNECT) {
    printf("  protocol_name: ");
    mqtt_buffer_dump(&(payload->connect.protocol_name));
    printf("\n");
  }
}
