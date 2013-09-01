#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <math.h>

#include "errors.h"
#include "message.h"

#include "parser.h"

#define READ_STRING(into) { \
    if ((len - *nread) < 2) { \
      return MQTT_PARSER_RC_INCOMPLETE; \
    } \
    \
    int str_length = data[*nread] * 256 + data[*nread + 1]; \
    \
    if ((len - *nread - 2) < str_length) { \
      return MQTT_PARSER_RC_INCOMPLETE; \
    } \
    \
    if (parser->buffer_pending == 0) { \
      parser->buffer_length = str_length; \
      \
      return MQTT_PARSER_RC_WANT_MEMORY; \
    } \
    \
    parser->buffer_pending = 0; \
    \
    if (parser->buffer != NULL) { \
      memcpy(parser->buffer, data + *nread + 2, fmin(str_length, parser->buffer_length)); \
      \
      into.length = fmin(str_length, parser->buffer_length); \
      into.data = parser->buffer; \
      \
      parser->buffer = NULL; \
      parser->buffer_length = 0; \
    } \
    \
    *nread += 2 + str_length; \
}

void mqtt_parser_init(mqtt_parser_t* parser) {
  parser->state = MQTT_PARSER_STATE_INITIAL;
  parser->buffer_pending = 0;
  parser->buffer = NULL;
  parser->buffer_length = 0;
}

void mqtt_parser_buffer(mqtt_parser_t* parser, uint8_t* buffer, size_t buffer_length) {
  parser->buffer_pending = 1;
  parser->buffer = buffer;
  parser->buffer_length = buffer_length;
}

int mqtt_parser_execute(mqtt_parser_t* parser, mqtt_message_t* message, uint8_t* data, size_t len, size_t* nread) {
  if (parser->state == MQTT_PARSER_STATE_INITIAL) {
    if ((len - *nread) < 1) {
      return MQTT_PARSER_RC_INCOMPLETE;
    }

    message->header.retain = (data[*nread + 0] >> 0) & 0x01;
    message->header.qos    = (data[*nread + 0] >> 1) & 0x03;
    message->header.dup    = (data[*nread + 0] >> 3) & 0x01;
    message->header.type   = (data[*nread + 0] >> 4) & 0x0f;

    message->payload.type  = message->header.type;

    *nread += 1;

    parser->state = MQTT_PARSER_STATE_REMAINING_LENGTH;

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
        message->header.length = value;

        *nread += digit_bytes;

        parser->state = MQTT_PARSER_STATE_VARIABLE_HEADER;

        return MQTT_PARSER_RC_CONTINUE;
      }
    } while (data[*nread + digit_bytes - 1] >= 0x80 && digit_bytes < 4);

    parser->error = MQTT_ERROR_PARSER_INVALID_REMAINING_LENGTH;

    return MQTT_PARSER_RC_ERROR;
  }

  if (parser->state == MQTT_PARSER_STATE_VARIABLE_HEADER) {
    if (message->header.type == MQTT_MESSAGE_TYPE_CONNECT) {
      parser->state = MQTT_PARSER_STATE_CONNECT_PROTOCOL_NAME;

      return MQTT_PARSER_RC_CONTINUE;
    }
  }

  if (parser->state == MQTT_PARSER_STATE_CONNECT_PROTOCOL_NAME) {
    READ_STRING(message->payload.connect.protocol_name)

    parser->state = MQTT_PARSER_STATE_CONNECT_PROTOCOL_VERSION;

    return MQTT_PARSER_RC_CONTINUE;
  }

  if (parser->state == MQTT_PARSER_STATE_CONNECT_PROTOCOL_VERSION) {
    if ((len - *nread) < 1) {
      return MQTT_PARSER_RC_INCOMPLETE;
    }

    message->payload.connect.protocol_version = data[*nread];

    *nread += 1;

    parser->state = MQTT_PARSER_STATE_CONNECT_FLAGS;

    return MQTT_PARSER_RC_CONTINUE;
  }

  if (parser->state == MQTT_PARSER_STATE_CONNECT_FLAGS) {
    if ((len - *nread) < 1) {
      return MQTT_PARSER_RC_INCOMPLETE;
    }

    message->payload.connect.flags.username_follows = (data[*nread] >> 7) & 0x01;
    message->payload.connect.flags.password_follows = (data[*nread] >> 6) & 0x01;
    message->payload.connect.flags.will_retain      = (data[*nread] >> 5) & 0x01;
    message->payload.connect.flags.will_qos         = (data[*nread] >> 4) & 0x02;
    message->payload.connect.flags.will             = (data[*nread] >> 2) & 0x01;
    message->payload.connect.flags.clean_session    = (data[*nread] >> 1) & 0x01;

    *nread += 1;

    parser->state = MQTT_PARSER_STATE_CONNECT_KEEP_ALIVE;

    return MQTT_PARSER_RC_CONTINUE;
  }

  if (parser->state == MQTT_PARSER_STATE_CONNECT_KEEP_ALIVE) {
    if ((len - *nread) < 2) {
      return MQTT_PARSER_RC_INCOMPLETE;
    }

    message->payload.connect.keep_alive = (data[*nread] << 8) + data[*nread + 1];

    *nread += 2;

    parser->state = MQTT_PARSER_STATE_CONNECT_CLIENT_IDENTIFIER;
  }

  if (parser->state == MQTT_PARSER_STATE_CONNECT_CLIENT_IDENTIFIER) {
    READ_STRING(message->payload.connect.client_identifier)

    parser->state = MQTT_PARSER_STATE_CONNECT_WILL_TOPIC;

    return MQTT_PARSER_RC_CONTINUE;
  }

  if (parser->state == MQTT_PARSER_STATE_CONNECT_WILL_TOPIC) {
    if (message->payload.connect.flags.will) {
      READ_STRING(message->payload.connect.will_topic)
    }

    parser->state = MQTT_PARSER_STATE_CONNECT_WILL_MESSAGE;

    return MQTT_PARSER_RC_CONTINUE;
  }

  if (parser->state == MQTT_PARSER_STATE_CONNECT_WILL_MESSAGE) {
    if (message->payload.connect.flags.will) {
      READ_STRING(message->payload.connect.will_message)
    }

    parser->state = MQTT_PARSER_STATE_CONNECT_USERNAME;

    return MQTT_PARSER_RC_CONTINUE;
  }

  if (parser->state == MQTT_PARSER_STATE_CONNECT_USERNAME) {
    if (message->payload.connect.flags.username_follows) {
      READ_STRING(message->payload.connect.username)
    }

    parser->state = MQTT_PARSER_STATE_CONNECT_PASSWORD;

    return MQTT_PARSER_RC_CONTINUE;
  }

  if (parser->state == MQTT_PARSER_STATE_CONNECT_PASSWORD) {
    if (message->payload.connect.flags.password_follows) {
      READ_STRING(message->payload.connect.password)
    }

    parser->state = MQTT_PARSER_STATE_INITIAL;

    return MQTT_PARSER_RC_DONE;
  }

  parser->error = MQTT_ERROR_PARSER_INVALID_STATE;

  return MQTT_PARSER_RC_ERROR;
}
