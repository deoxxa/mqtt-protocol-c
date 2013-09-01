#include <stdio.h>

#include "header.h"

void mqtt_header_init(mqtt_header_t* header) {
  header->retain = 0;
  header->qos = 0;
  header->dup = 0;
  header->type = 0;
  header->length = 0;
}

void mqtt_header_dump(mqtt_header_t* header) {
  printf("header\n");
  printf("  retain: %s\n", header->retain ? "true": "false");
  printf("  qos:    %d\n", header->qos);
  printf("  dup:    %s\n", header->dup ? "true" : "false");
  printf("  type:   %d\n", header->type);
  printf("  length: %d\n", header->length);
}
