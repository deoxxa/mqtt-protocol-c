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

mqtt_parser_rc_t mqtt_parser_execute(mqtt_parser_t* parser, mqtt_message_t* message, uint8_t* data, size_t len, size_t* nread) {
  do {
    switch (parser->state) {
      case MQTT_PARSER_STATE_INITIAL: {
        if ((len - *nread) < 1) {
          return MQTT_PARSER_RC_INCOMPLETE;
        }

        message->common.retain = (data[*nread + 0] >> 0) & 0x01;
        message->common.qos    = (data[*nread + 0] >> 1) & 0x03;
        message->common.dup    = (data[*nread + 0] >> 3) & 0x01;
        message->common.type   = (data[*nread + 0] >> 4) & 0x0f;

        *nread += 1;

        parser->state = MQTT_PARSER_STATE_REMAINING_LENGTH;

        break;
      }

      case MQTT_PARSER_STATE_REMAINING_LENGTH: {
        int digit_bytes = 0,
            multiplier = 1,
            remaining_length = 0;

        do {
          digit_bytes += 1;

          if ((len - *nread) < digit_bytes) {
            return MQTT_PARSER_RC_INCOMPLETE;
          }

          remaining_length += (data[*nread + digit_bytes - 1] & 0x7f) * multiplier;
          multiplier *= 128;
        } while (data[*nread + digit_bytes - 1] >= 0x80 && digit_bytes < 4);

        if (data[*nread + digit_bytes - 1] >= 0x80) {
          parser->error = MQTT_ERROR_PARSER_INVALID_REMAINING_LENGTH;

          return MQTT_PARSER_RC_ERROR;
        }

        message->common.length = remaining_length;

        *nread += digit_bytes;

        switch (message->common.type) {
          case MQTT_TYPE_CONNECT: {
            parser->state = MQTT_PARSER_STATE_CONNECT;
            break;
          }
          case MQTT_TYPE_CONNACK: {
            parser->state = MQTT_PARSER_STATE_CONNACK;
            break;
          }
          case MQTT_TYPE_PUBLISH: {
            parser->state = MQTT_PARSER_STATE_PUBACK;
            break;
          }
          case MQTT_TYPE_PUBREC: {
            parser->state = MQTT_PARSER_STATE_PUBREC;
            break;
          }
          case MQTT_TYPE_PUBREL: {
            parser->state = MQTT_PARSER_STATE_PUBREL;
            break;
          }
          case MQTT_TYPE_PUBCOMP: {
            parser->state = MQTT_PARSER_STATE_PUBCOMP;
            break;
          }
          default: {
            parser->error = MQTT_ERROR_PARSER_INVALID_MESSAGE_ID;
            return MQTT_PARSER_RC_ERROR;
          }
        }

        break;
      }

      case MQTT_PARSER_STATE_VARIABLE_HEADER: {
        if (message->common.type == MQTT_TYPE_CONNECT) {
          parser->state = MQTT_PARSER_STATE_CONNECT_PROTOCOL_NAME;
        }

        break;
      }

      case MQTT_PARSER_STATE_CONNECT: {
        parser->state = MQTT_PARSER_STATE_CONNECT_PROTOCOL_NAME;

        break;
      }

      case MQTT_PARSER_STATE_CONNECT_PROTOCOL_NAME: {
        READ_STRING(message->connect.protocol_name)

        parser->state = MQTT_PARSER_STATE_CONNECT_PROTOCOL_VERSION;

        break;
      }

      case MQTT_PARSER_STATE_CONNECT_PROTOCOL_VERSION: {
        if ((len - *nread) < 1) {
          return MQTT_PARSER_RC_INCOMPLETE;
        }

        message->connect.protocol_version = data[*nread];

        *nread += 1;

        parser->state = MQTT_PARSER_STATE_CONNECT_FLAGS;

        break;
      }

      case MQTT_PARSER_STATE_CONNECT_FLAGS: {
        if ((len - *nread) < 1) {
          return MQTT_PARSER_RC_INCOMPLETE;
        }

        message->connect.flags.username_follows = (data[*nread] >> 7) & 0x01;
        message->connect.flags.password_follows = (data[*nread] >> 6) & 0x01;
        message->connect.flags.will_retain      = (data[*nread] >> 5) & 0x01;
        message->connect.flags.will_qos         = (data[*nread] >> 4) & 0x02;
        message->connect.flags.will             = (data[*nread] >> 2) & 0x01;
        message->connect.flags.clean_session    = (data[*nread] >> 1) & 0x01;

        *nread += 1;

        parser->state = MQTT_PARSER_STATE_CONNECT_KEEP_ALIVE;

        break;
      }

      case MQTT_PARSER_STATE_CONNECT_KEEP_ALIVE: {
        if ((len - *nread) < 2) {
          return MQTT_PARSER_RC_INCOMPLETE;
        }

        message->connect.keep_alive = (data[*nread] << 8) + data[*nread + 1];

        *nread += 2;

        parser->state = MQTT_PARSER_STATE_CONNECT_CLIENT_IDENTIFIER;

        break;
      }

      case MQTT_PARSER_STATE_CONNECT_CLIENT_IDENTIFIER: {
        READ_STRING(message->connect.client_id)

        parser->state = MQTT_PARSER_STATE_CONNECT_WILL_TOPIC;

        break;
      }

      case MQTT_PARSER_STATE_CONNECT_WILL_TOPIC: {
        if (message->connect.flags.will) {
          READ_STRING(message->connect.will_topic)
        }

        parser->state = MQTT_PARSER_STATE_CONNECT_WILL_MESSAGE;

        break;
      }

      case MQTT_PARSER_STATE_CONNECT_WILL_MESSAGE: {
        if (message->connect.flags.will) {
          READ_STRING(message->connect.will_message)
        }

        parser->state = MQTT_PARSER_STATE_CONNECT_USERNAME;

        break;
      }

      case MQTT_PARSER_STATE_CONNECT_USERNAME: {
        if (message->connect.flags.username_follows) {
          READ_STRING(message->connect.username)
        }

        parser->state = MQTT_PARSER_STATE_CONNECT_PASSWORD;

        break;
      }

      case MQTT_PARSER_STATE_CONNECT_PASSWORD: {
        if (message->connect.flags.password_follows) {
          READ_STRING(message->connect.password)
        }

        parser->state = MQTT_PARSER_STATE_INITIAL;

        return MQTT_PARSER_RC_DONE;
      }

      case MQTT_PARSER_STATE_CONNACK: {
        if ((len - *nread) < 2) {
          return MQTT_PARSER_RC_INCOMPLETE;
        }

        message->connack._unused     = data[*nread];
        message->connack.return_code = data[*nread + 1];

        *nread += 2;

        parser->state = MQTT_PARSER_STATE_INITIAL;

        return MQTT_PARSER_RC_DONE;
      }

      case MQTT_PARSER_STATE_PUBLISH: {
        parser->state = MQTT_PARSER_STATE_PUBLISH_TOPIC_NAME;

        break;
      }

      case MQTT_PARSER_STATE_PUBLISH_TOPIC_NAME: {
        READ_STRING(message->publish.topic_name)

        parser->state = MQTT_PARSER_STATE_PUBLISH_MESSAGE_ID;

        break;
      }

      case MQTT_PARSER_STATE_PUBLISH_MESSAGE_ID: {
        if ((len - *nread) < 2) {
          return MQTT_PARSER_RC_INCOMPLETE;
        }

        message->publish.message_id = (data[*nread] << 8) + data[*nread + 1];

        *nread += 2;

        parser->state = MQTT_PARSER_STATE_INITIAL;

        return MQTT_PARSER_RC_DONE;
      }

      case MQTT_PARSER_STATE_PUBACK: {
        if ((len - *nread) < 2) {
          return MQTT_PARSER_RC_INCOMPLETE;
        }

        message->puback.message_id = (data[*nread] << 8) + data[*nread + 1];

        *nread += 2;

        parser->state = MQTT_PARSER_STATE_INITIAL;

        return MQTT_PARSER_RC_DONE;
      }

      case MQTT_PARSER_STATE_PUBREC: {
        if ((len - *nread) < 2) {
          return MQTT_PARSER_RC_INCOMPLETE;
        }

        message->pubrec.message_id = (data[*nread] << 8) + data[*nread + 1];

        *nread += 2;

        parser->state = MQTT_PARSER_STATE_INITIAL;

        return MQTT_PARSER_RC_DONE;
      }

      case MQTT_PARSER_STATE_PUBREL: {
        if ((len - *nread) < 2) {
          return MQTT_PARSER_RC_INCOMPLETE;
        }

        message->pubrel.message_id = (data[*nread] << 8) + data[*nread + 1];

        *nread += 2;

        parser->state = MQTT_PARSER_STATE_INITIAL;

        return MQTT_PARSER_RC_DONE;
      }

      case MQTT_PARSER_STATE_PUBCOMP: {
        if ((len - *nread) < 2) {
          return MQTT_PARSER_RC_INCOMPLETE;
        }

        message->pubcomp.message_id = (data[*nread] << 8) + data[*nread + 1];

        *nread += 2;

        parser->state = MQTT_PARSER_STATE_INITIAL;

        return MQTT_PARSER_RC_DONE;
      }

      default: {
        parser->error = MQTT_ERROR_PARSER_INVALID_STATE;

        return MQTT_PARSER_RC_ERROR;
      }
    }
  } while (1);
}
