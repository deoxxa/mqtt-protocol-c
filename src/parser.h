#ifndef MQTT_PARSER_H
#define MQTT_PARSER_H

#include <stdint.h>
#include <stddef.h>

#include "errors.h"
#include "message.h"

typedef enum mqtt_parser_rc_e {
  MQTT_PARSER_RC_ERROR,
  MQTT_PARSER_RC_CONTINUE,
  MQTT_PARSER_RC_INCOMPLETE,
  MQTT_PARSER_RC_DONE,
  MQTT_PARSER_RC_WANT_MEMORY,
} mqtt_parser_rc_t;

typedef enum mqtt_parser_state_e {
  MQTT_PARSER_STATE_INITIAL,
  MQTT_PARSER_STATE_REMAINING_LENGTH,
  MQTT_PARSER_STATE_VARIABLE_HEADER,
  MQTT_PARSER_STATE_CONNECT,
  MQTT_PARSER_STATE_CONNECT_PROTOCOL_NAME,
  MQTT_PARSER_STATE_CONNECT_PROTOCOL_VERSION,
  MQTT_PARSER_STATE_CONNECT_FLAGS,
  MQTT_PARSER_STATE_CONNECT_KEEP_ALIVE,
  MQTT_PARSER_STATE_CONNECT_CLIENT_IDENTIFIER,
  MQTT_PARSER_STATE_CONNECT_WILL_TOPIC,
  MQTT_PARSER_STATE_CONNECT_WILL_MESSAGE,
  MQTT_PARSER_STATE_CONNECT_USERNAME,
  MQTT_PARSER_STATE_CONNECT_PASSWORD,
  MQTT_PARSER_STATE_CONNACK,
  MQTT_PARSER_STATE_PUBLISH,
  MQTT_PARSER_STATE_PUBLISH_TOPIC_NAME,
  MQTT_PARSER_STATE_PUBLISH_MESSAGE_ID,
  MQTT_PARSER_STATE_PUBACK,
  MQTT_PARSER_STATE_PUBREC,
  MQTT_PARSER_STATE_PUBREL,
  MQTT_PARSER_STATE_PUBCOMP,
} mqtt_parser_state_t;

typedef struct mqtt_parser_s {
  mqtt_parser_state_t state;
  mqtt_error_t error;
  char buffer_pending;
  uint8_t* buffer;
  size_t buffer_length;
} mqtt_parser_t;

void mqtt_parser_init(mqtt_parser_t* parser);
void mqtt_parser_buffer(mqtt_parser_t* parser, uint8_t* buffer, size_t buffer_length);
mqtt_parser_rc_t mqtt_parser_execute(mqtt_parser_t* parser, mqtt_message_t* message, uint8_t* data, size_t len, size_t* nread);

#endif
