#include <stdio.h>
#include <stdint.h>

#include "message.h"
#include "parser.h"
#include "errors.h"

int main(int argc, char** argv) {
  mqtt_parser_t parser;
  mqtt_message_t message;
  uint8_t data[4] = {0x14, 0x80, 0x01, 0x00};

  mqtt_parser_init(&parser);

  size_t nread = 0;
  int rc = 0;

  printf("parser runs\n");
  do {
    printf("state: %d, ", parser.state);
    rc = mqtt_parser_execute(&parser, &message, data, 4, &nread);
    printf("rc: %d\n", rc);
  } while (rc == MQTT_PARSER_RC_CONTINUE);

  printf("\n");
  printf("parser info\n");
  printf("rc: %d\n", rc);
  if (rc == MQTT_PARSER_RC_ERROR) {
    printf("error: %s\n", mqtt_error_string(parser.error));
  }
  printf("nread: %zd\n", nread);
  printf("\n");

  printf("message info\n");
  printf("type: %d\n", message.type);
  printf("qos: %d\n", message.qos);
  printf("dup: %d\n", message.dup);
  printf("retain: %d\n", message.retain);
  printf("length: %d\n", message.length);
  printf("\n");

  return 0;
}
