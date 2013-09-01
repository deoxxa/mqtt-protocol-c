#include <stdint.h>
#include <stddef.h>

#include "errors.h"
#include "message.h"

#include "parser.h"

void mqtt_parser_init(mqtt_parser_t* parser) {
  parser->state = MQTT_PARSER_STATE_INITIAL;
}

int mqtt_parser_execute(mqtt_parser_t* parser, mqtt_message_t* message, uint8_t* data, size_t len, size_t* nread) {
  if (parser->state == MQTT_PARSER_STATE_INITIAL) {
    if ((len - *nread) < 1) {
      return MQTT_PARSER_RC_INCOMPLETE;
    }

    message->retain = (data[*nread + 0] >> 0) & 0x01;
    message->qos    = (data[*nread + 0] >> 1) & 0x03;
    message->dup    = (data[*nread + 0] >> 3) & 0x01;
    message->type   = (data[*nread + 0] >> 4) & 0x0f;

    parser->state = MQTT_PARSER_STATE_REMAINING_LENGTH;

    *nread += 1;

    return MQTT_PARSER_RC_CONTINUE;
  }

  if (parser->state == MQTT_PARSER_STATE_REMAINING_LENGTH) {
    int digit_bytes = 0,
        multiplier = 1,
        value = 0;

    do {
      digit_bytes += 1;

      if ((len - *nread) < digit_bytes) {
        return MQTT_PARSER_RC_INCOMPLETE;
      }

      value += (data[*nread + digit_bytes - 1] & 0x7f) * multiplier;
      multiplier *= 128;

      if (data[*nread + digit_bytes - 1] <= 0x7f) {
        message->length = value;
        parser->state = MQTT_PARSER_STATE_VARIABLE_HEADER;
        *nread += digit_bytes;
        return MQTT_PARSER_RC_CONTINUE;
      }
    } while (data[*nread + digit_bytes - 1] >= 0x80 && digit_bytes < 4);

    parser->error = MQTT_ERROR_PARSER_INVALID_REMAINING_LENGTH;

    return MQTT_PARSER_RC_ERROR;
  }

  parser->error = MQTT_ERROR_PARSER_INVALID_STATE;

  return MQTT_PARSER_RC_ERROR;
}
