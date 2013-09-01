#include <string.h>

#include "message.h"

#include "serialiser.h"

void mqtt_serialiser_init(mqtt_serialiser_t* serialiser) {
  memset(serialiser, 0, sizeof (mqtt_serialiser_t));
}
