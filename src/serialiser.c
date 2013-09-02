#include <string.h>

#include "message.h"

#include "serialiser.h"

#define WRITE_STRING(name) { \
  buffer[offset++] = name.length / 0xff; \
  buffer[offset++] = name.length & 0xff; \
  memcpy(&(buffer[offset]), name.data, name.length); \
  offset += name.length; \
}

void mqtt_serialiser_init(mqtt_serialiser_t* serialiser) {
  memset(serialiser, 0, sizeof (mqtt_serialiser_t));
}

size_t mqtt_serialiser_size(mqtt_serialiser_t* serialiser, mqtt_message_t* message) {
  size_t len = 1;

  if (message->header.length <= 127) {
    len += 1;
  } else if (message->header.length <= 16383) {
    len += 2;
  } else if (message->header.length <= 2097151) {
    len += 3;
  } else if (message->header.length <= 268435455) {
    len += 4;
  }

  if (message->header.type == MQTT_MESSAGE_TYPE_CONNECT) {
    len += 8;

    len += message->payload.connect.protocol_name.length;
    len += message->payload.connect.client_identifier.length;

    if (message->payload.connect.flags.username_follows) {
      len += 2;
      len += message->payload.connect.username.length;
    }

    if (message->payload.connect.flags.password_follows) {
      len += 2;
      len += message->payload.connect.password.length;
    }

    if (message->payload.connect.flags.will) {
      len += 4;
      len += message->payload.connect.will_topic.length;
      len += message->payload.connect.will_message.length;
    }
  }

  return len;
}

int mqtt_serialiser_write(mqtt_serialiser_t* serialiser, mqtt_message_t* message, uint8_t* buffer, size_t len) {
  unsigned int offset = 0;

  buffer[offset++] = message->header.retain + (message->header.qos << 1) + (message->header.dup << 3) + (message->header.type << 4);

  uint32_t remaining_length = message->header.length;
  do {
    buffer[offset++] = remaining_length & 0x7f;
    remaining_length >>= 7;
  } while (remaining_length > 0);

  if (message->header.type == MQTT_MESSAGE_TYPE_CONNECT) {
    WRITE_STRING(message->payload.connect.protocol_name);

    buffer[offset++] = message->payload.connect.protocol_version;

    buffer[offset++] =
      (message->payload.connect.flags.username_follows << 7) +
      (message->payload.connect.flags.password_follows << 6) +
      (message->payload.connect.flags.will_retain      << 5) +
      (message->payload.connect.flags.will_qos         << 3) +
      (message->payload.connect.flags.will             << 2) +
      (message->payload.connect.flags.clean_session    << 1);

    buffer[offset++] = message->payload.connect.keep_alive >> 8;
    buffer[offset++] = message->payload.connect.keep_alive & 0xff;

    WRITE_STRING(message->payload.connect.client_identifier);

    if (message->payload.connect.flags.will) {
      WRITE_STRING(message->payload.connect.will_topic);
      WRITE_STRING(message->payload.connect.will_message);
    }

    if (message->payload.connect.flags.username_follows) {
      WRITE_STRING(message->payload.connect.username);
    }

    if (message->payload.connect.flags.password_follows) {
      WRITE_STRING(message->payload.connect.password);
    }
  }

  return 0;
}
