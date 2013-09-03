#ifndef MQTT_ERRORS_H
#define MQTT_ERRORS_H

typedef enum mqtt_error_e {
  MQTT_ERROR_NONE = 0,
  MQTT_ERROR_PARSER_INVALID_STATE = 1,
  MQTT_ERROR_PARSER_INVALID_REMAINING_LENGTH = 2,
  MQTT_ERROR_PARSER_INVALID_MESSAGE_ID = 3,
  MQTT_ERROR_SERIALISER_INVALID_MESSAGE_ID = 4,
} mqtt_error_t;

const char* mqtt_error_string(mqtt_error_t error);

#endif
