#include <string.h>
#include <stdio.h>

#include "message.h"

void mqtt_message_init(mqtt_message_t* message) {
  memset(message, 0, sizeof (mqtt_message_t));
}

void mqtt_message_dump(mqtt_message_t* message) {
  printf("message\n");
  printf("  type:              %d\n", message->common.type);
  printf("  qos:               %d\n", message->common.qos);
  printf("  dup:               %s\n", message->common.dup    ? "true" : "false");
  printf("  retain:            %s\n", message->common.retain ? "true" : "false");

  if (message->common.type == MQTT_TYPE_CONNECT) {
    printf("  protocol name:     ");
    mqtt_buffer_dump(&(message->connect.protocol_name));
    printf("\n");

    printf("  protocol version:  %d\n", message->connect.protocol_version);

    printf("  has username:      %s\n", message->connect.flags.username_follows ? "true": "false");
    printf("  has password:      %s\n", message->connect.flags.password_follows ? "true": "false");
    printf("  has will:          %s\n", message->connect.flags.will ? "true": "false");
    printf("  will qos:          %d\n", message->connect.flags.will_qos);
    printf("  retains will:      %s\n", message->connect.flags.will_retain ? "true": "false");
    printf("  clean session:     %s\n", message->connect.flags.clean_session ? "true": "false");

    printf("  keep alive:        %d\n", message->connect.keep_alive);

    printf("  client id:         ");
    mqtt_buffer_dump(&(message->connect.client_id));
    printf("\n");

    printf("  will topic:        ");
    mqtt_buffer_dump(&(message->connect.will_topic));
    printf("\n");
    printf("  will message:      ");
    mqtt_buffer_dump(&(message->connect.will_message));
    printf("\n");

    printf("  username:          ");
    mqtt_buffer_dump(&(message->connect.username));
    printf("\n");
    printf("  password:          ");
    mqtt_buffer_dump(&(message->connect.password));
    printf("\n");
  }
}
