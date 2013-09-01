#ifndef MQTT_HEADER_H
#define MQTT_HEADER_H

#include <stdint.h>

#include "qos.h"

typedef enum mqtt_header_type_e {
  MQTT_MESSAGE_TYPE_CONNECT = 1,
  MQTT_MESSAGE_TYPE_CONNACK = 2,
  MQTT_MESSAGE_TYPE_PUBLISH = 3,
  MQTT_MESSAGE_TYPE_PUBACK = 4,
  MQTT_MESSAGE_TYPE_PUBREC = 5,
  MQTT_MESSAGE_TYPE_PUBREL = 6,
  MQTT_MESSAGE_TYPE_PUBCOMP = 7,
  MQTT_MESSAGE_TYPE_SUBSCRIBE = 8,
  MQTT_MESSAGE_TYPE_SUBACK = 9,
  MQTT_MESSAGE_TYPE_UNSUBSCRIBE = 10,
  MQTT_MESSAGE_TYPE_UNSUBACK = 11,
  MQTT_MESSAGE_TYPE_PINGREQ = 12,
  MQTT_MESSAGE_TYPE_PINGRESP = 13,
  MQTT_MESSAGE_TYPE_DISCONNECT = 14
} mqtt_header_type_t;

typedef enum mqtt_header_retain_e {
  MQTT_MESSAGE_RETAIN_FALSE = 0,
  MQTT_MESSAGE_RETAIN_TRUE = 1
} mqtt_header_retain_t;

typedef enum mqtt_header_dup_e {
  MQTT_MESSAGE_DUP_FALSE = 0,
  MQTT_MESSAGE_DUP_TRUE = 1
} mqtt_header_dup_t;

typedef struct mqtt_header_s {
  mqtt_header_retain_t retain;
  mqtt_qos_t qos;
  mqtt_header_dup_t dup;
  mqtt_header_type_t type;
  uint32_t length;
} mqtt_header_t;

void mqtt_header_init(mqtt_header_t* header);
void mqtt_header_dump(mqtt_header_t* header);

#endif
