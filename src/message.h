#ifndef MQTT_MESSAGE_H
#define MQTT_MESSAGE_H

#include <stdint.h>

typedef enum mqtt_message_type_e {
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
} mqtt_message_type_t;

typedef enum mqtt_message_qos_e {
  MQTT_MESSAGE_QOS_AT_MOST_ONCE = 0,
  MQTT_MESSAGE_QOS_AT_LEAST_ONCE = 1,
  MQTT_MESSAGE_QOS_EXACTLY_ONCE = 2
} mqtt_message_qos_t;

typedef enum mqtt_message_retain_e {
  MQTT_MESSAGE_RETAIN_FALSE = 0,
  MQTT_MESSAGE_RETAIN_TRUE = 1
} mqtt_message_retain_t;

typedef enum mqtt_message_dup_e {
  MQTT_MESSAGE_DUP_FALSE = 0,
  MQTT_MESSAGE_DUP_TRUE = 1
} mqtt_message_dup_t;

typedef struct mqtt_message_s {
  mqtt_message_retain_t retain:1;
  mqtt_message_qos_t qos:2;
  mqtt_message_dup_t dup:1;
  mqtt_message_type_t type:4;
  uint32_t length;
} mqtt_message_t;

#endif
