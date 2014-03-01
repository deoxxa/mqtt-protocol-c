mqtt-protocol-c
===============

Zero-copy, interruptible MQTT protocol parser and serialiser written in C

Overview
--------

mqtt-protocol-c is designed for use in resource-constrained environments. To
that end, it avoids making any memory allocations or assumptions about the
underlying system. It handles only the binary parsing/serialising part of the
MQTT protocol, leaving all the logic and networking up to the user.

API
---

Please see the header and implementation files for now to see how these all fit
together.

### mqtt_buffer_dump

```c
void mqtt_buffer_dump(mqtt_buffer_t* buffer);
```

### mqtt_buffer_dump_ascii

```c
void mqtt_buffer_dump_ascii(mqtt_buffer_t* buffer);
```

### mqtt_buffer_dump_hex

```c
void mqtt_buffer_dump_hex(mqtt_buffer_t* buffer);
```

### mqtt_error_string

```c
const char* mqtt_error_string(mqtt_error_t error);
```

### mqtt_message_init

```c
void mqtt_message_init(mqtt_message_t* message);
```

### mqtt_message_dump

```c
void mqtt_message_dump(mqtt_message_t* message);
```

### mqtt_parser_init

```c
void mqtt_parser_init(mqtt_parser_t* parser);
```

### mqtt_parser_buffer

```c
void mqtt_parser_buffer(mqtt_parser_t* parser, uint8_t* buffer, size_t buffer_length);
```

### mqtt_parser_execute

```c
mqtt_parser_rc_t mqtt_parser_execute(mqtt_parser_t* parser, mqtt_message_t* message, uint8_t* data, size_t len, size_t* nread);
```

### mqtt_serialiser_init

```c
void mqtt_serialiser_init(mqtt_serialiser_t* serialiser);
```

### mqtt_serialiser_size

```c
size_t mqtt_serialiser_size(mqtt_serialiser_t* serialiser, mqtt_message_t* message);
```

### mqtt_serialiser_write

```c
mqtt_serialiser_rc_t mqtt_serialiser_write(mqtt_serialiser_t* serialiser, mqtt_message_t* message, uint8_t* buffer, size_t len);
```

License
-------

3-clause BSD. A copy is included with the source.

Contact
-------

* GitHub ([deoxxa](http://github.com/deoxxa))
* Twitter ([@deoxxa](http://twitter.com/deoxxa))
* Email ([deoxxa@fknsrs.biz](mailto:deoxxa@fknsrs.biz))
