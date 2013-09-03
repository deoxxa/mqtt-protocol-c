#ifndef MQTT_ERRORS_H
#define MQTT_ERRORS_H

typedef enum mqtt_error_e {
  MQTT_ERROR_PARSER_INVALID_STATE,
  MQTT_ERROR_PARSER_INVALID_REMAINING_LENGTH,
  MQTT_ERROR_PARSER_INVALID_MESSAGE_ID,
} mqtt_error_t;

const char* mqtt_error_string(mqtt_error_t error);

#endif
