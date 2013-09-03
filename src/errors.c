#include <stddef.h>

#include "errors.h"

const char* mqtt_error_strings[] = {
  NULL,
  "[parser] left in an invalid state",
  "[parser] remaining length field was invalid",
  "[parser] message id was invalid",
  "[serialiser] message if was invalid",
};

const char* mqtt_error_string(mqtt_error_t error) {
  return mqtt_error_strings[error];
}
