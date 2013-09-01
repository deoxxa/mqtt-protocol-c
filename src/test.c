#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "message.h"
#include "parser.h"
#include "errors.h"

int main(int argc, char** argv) {
  mqtt_parser_t parser;
  mqtt_message_t message;
  uint8_t data[] = {
    // type: 1
    0x10,
    // length: 12
    0x0c,
      // protocol name (string)
      0x00, 0x06, // length: 6
        // data
        0x4d, 0x51, 0x49, 0x73, 0x64, 0x70,
      // protocol verson
      0x03,
      // connect flags
      0x00,
      // timeout
      0x00, 0x10,
  };

  mqtt_parser_init(&parser);
  mqtt_message_init(&message);

  size_t nread = 0;
  int rc = 0, loops = 0;

  printf("parser running (%zu bytes)\n", sizeof data);
  do {
    printf("  loop %d\n", ++loops);
    printf("    state: %d\n", parser.state);
    printf("    offset: %zu\n", nread);
    rc = mqtt_parser_execute(&parser, &message, data, sizeof data, &nread);
    printf("    rc: %d\n", rc);

    if (rc == MQTT_PARSER_RC_WANT_MEMORY) {
      printf("    bytes requested: %zu\n", parser.buffer_length);
      mqtt_parser_buffer(&parser, malloc(parser.buffer_length), parser.buffer_length);
    }
  } while (rc == MQTT_PARSER_RC_CONTINUE || rc == MQTT_PARSER_RC_WANT_MEMORY);

  printf("\n");
  printf("parser info\n");
  printf("  state: %d\n", parser.state);
  if (rc == MQTT_PARSER_RC_ERROR) {
    printf("  error: %s\n", mqtt_error_string(parser.error));
  }
  printf("  nread: %zd\n", nread);
  printf("  loops: %d\n", loops);
  printf("\n");

  printf("message info\n");
  printf("\n");
  mqtt_message_dump(&message);

  return 0;
}
