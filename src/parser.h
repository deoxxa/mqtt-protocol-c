#ifndef MQTT_PARSER_H
#define MQTT_PARSER_H

#include <stdint.h>
#include <stddef.h>

#include "errors.h"
#include "message.h"

#define MQTT_PARSER_RC_ERROR -1
#define MQTT_PARSER_RC_CONTINUE 0
#define MQTT_PARSER_RC_INCOMPLETE 1
#define MQTT_PARSER_RC_DONE 2

typedef enum mqtt_parser_state_e {
  MQTT_PARSER_STATE_INITIAL = 0,
  MQTT_PARSER_STATE_REMAINING_LENGTH = 1,
  MQTT_PARSER_STATE_VARIABLE_HEADER = 2
} mqtt_parser_state_t;

typedef struct mqtt_parser_s {
  mqtt_parser_state_t state;
  mqtt_error_t error;
} mqtt_parser_t;

void mqtt_parser_init(mqtt_parser_t* parser);
int mqtt_parser_execute(mqtt_parser_t* parser, mqtt_message_t* message, uint8_t* data, size_t len, size_t* nread);

#endif
