#ifndef MQTT_QOS_H
#define MQTT_QOS_H

typedef enum mqtt_qos_e {
  MQTT_QOS_AT_MOST_ONCE = 0,
  MQTT_QOS_AT_LEAST_ONCE = 1,
  MQTT_QOS_EXACTLY_ONCE = 2,
} mqtt_qos_t;

#endif
