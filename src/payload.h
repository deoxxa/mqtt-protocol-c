#ifndef MQTT_PAYLOAD_H
#define MQTT_PAYLOAD_H

#include <stdint.h>

#include "header.h"
#include "buffer.h"

#define MQTT_PAYLOAD_COMMON_FIELDS \
  mqtt_header_type_t type;

typedef struct mqtt_payload_connect_s {
  MQTT_PAYLOAD_COMMON_FIELDS

  mqtt_buffer_t protocol_name;
  uint8_t protocol_version;

  union {
    uint8_t raw;

    struct {
      unsigned char username_follows:1;
      unsigned char password_follows:1;
      unsigned char will_follows:1;
      unsigned char will_qos:2;
      unsigned char clean_session:1;
    } named;
  } flags;

  uint16_t keep_alive;

  mqtt_buffer_t client_identifier;

  mqtt_buffer_t will_topic;
  mqtt_buffer_t will_message;

  mqtt_buffer_t username;
  mqtt_buffer_t password;
} mqtt_payload_connect_t;

typedef union mqtt_payload_u {
  mqtt_header_type_t type;
  mqtt_payload_connect_t connect;
} mqtt_payload_t;

void mqtt_payload_init(mqtt_payload_t* payload);
void mqtt_payload_dump(mqtt_payload_t* payload);

#endif
