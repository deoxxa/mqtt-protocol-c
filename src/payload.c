#include <stdio.h>

#include "header.h"

#include "payload.h"

void mqtt_payload_init(mqtt_payload_t* payload) {
}

void mqtt_payload_dump(mqtt_payload_t* payload) {
  printf("payload\n");
  printf("  type:              %d\n", payload->type);

  if (payload->type == MQTT_MESSAGE_TYPE_CONNECT) {
    printf("  protocol name:     ");
    mqtt_buffer_dump(&(payload->connect.protocol_name));
    printf("\n");

    printf("  protocol version:  %d\n", payload->connect.protocol_version);

    printf("  has username:      %s\n", payload->connect.flags.named.username_follows ? "true": "false");
    printf("  has password:      %s\n", payload->connect.flags.named.password_follows ? "true": "false");
    printf("  has will:          %s\n", payload->connect.flags.named.will_follows ? "true": "false");
    printf("  will qos:          %d\n", payload->connect.flags.named.will_qos);
    printf("  clean session:     %s\n", payload->connect.flags.named.clean_session ? "true": "false");

    printf("  keep alive:        %d\n", payload->connect.keep_alive);

    printf("  client identifier: ");
    mqtt_buffer_dump(&(payload->connect.client_identifier));
    printf("\n");

    printf("  will topic:        ");
    mqtt_buffer_dump(&(payload->connect.will_topic));
    printf("\n");
    printf("  will message:      ");
    mqtt_buffer_dump(&(payload->connect.will_message));
    printf("\n");

    printf("  username:          ");
    mqtt_buffer_dump(&(payload->connect.username));
    printf("\n");
    printf("  password:          ");
    mqtt_buffer_dump(&(payload->connect.password));
    printf("\n");
  }
}
