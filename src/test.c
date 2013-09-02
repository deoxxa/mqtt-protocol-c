#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "message.h"
#include "parser.h"
#include "serialiser.h"
#include "errors.h"

int main(int argc, char** argv) {
  mqtt_parser_t parser;
  mqtt_serialiser_t serialiser;
  mqtt_message_t message;

/*
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
*/

  uint8_t data[] = {
    // type 1
    0x10,
    // length 94
    0x5e,
      // protocol name
      0x00, 0x06,
        0x4d, 0x51, 0x49, 0x73, 0x64, 0x70,
      // protocol version
      0x03,
      // flags
      0xf6,
      // keep-alive
      0x00, 0x1e,
      // client id
      0x00, 0x05,
        0x68, 0x65, 0x6c, 0x6c, 0x6f,
      // will topic
      0x00, 0x06,
        0x73, 0x68, 0x6f, 0x75, 0x74, 0x73,
      // will message
      0x00, 0x16,
        0x59, 0x4f, 0x20, 0x57, 0x48, 0x41, 0x54, 0x27, 0x53, 0x20, 0x55, 0x50, 0x20, 0x4d, 0x59, 0x20, 0x48, 0x4f, 0x4d, 0x49, 0x45, 0x53,
      // username
      0x00, 0x07,
        0x74, 0x68, 0x65, 0x75, 0x73, 0x65, 0x72,
      // password (md5)
      0x00, 0x20,
        0x62, 0x61, 0x33, 0x63, 0x38, 0x33, 0x33, 0x34, 0x38, 0x62, 0x64, 0x64, 0x66, 0x37, 0x62, 0x33, 0x36, 0x38, 0x62, 0x34, 0x37, 0x38, 0x61, 0x63, 0x30, 0x36, 0x64, 0x33, 0x33, 0x34, 0x30, 0x65,
  };

  mqtt_parser_init(&parser);
  mqtt_serialiser_init(&serialiser);
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

  mqtt_message_dump(&message);

  printf("\n");

  size_t packet_length = mqtt_serialiser_size(&serialiser, &message);
  uint8_t* packet = malloc(packet_length);
  mqtt_serialiser_write(&serialiser, &message, packet, packet_length);

  printf("packet length: %zu\n", packet_length);
  printf("packet data:   ");
  for (int i=0;i<packet_length;++i) {
    printf("%02x ", packet[i]);
  }
  printf("\n");

  printf("\n");
  printf("difference: %d\n", memcmp(data, packet, packet_length));

  return 0;
}
