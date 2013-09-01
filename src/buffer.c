#include <stdio.h>

#include "buffer.h"

void mqtt_buffer_dump(mqtt_buffer_t* buffer) {
  printf("[%d] ", buffer->length);

  for (int i=0;i<buffer->length;++i) {
    printf("%02x ", buffer->data[i]);
  }
}
