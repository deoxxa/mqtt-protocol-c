CFLAGS += -Wall -Werror -pedantic

all: bin/test

clean:
	rm src/*.o bin/*

src/errors.o: src/errors.c src/errors.h
src/buffer.o: src/buffer.c src/buffer.h
src/header.o: src/header.c src/header.h
src/payload.o: src/payload.c src/payload.h
src/message.o: src/message.c src/message.h
src/parser.o: src/parser.c src/parser.h
src/serialiser.o: src/serialiser.c src/serialiser.h
src/test.o: src/test.c

bin/test: src/errors.o src/buffer.o src/header.o src/payload.o src/message.o src/parser.o src/serialiser.o src/test.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o bin/test src/errors.o src/buffer.o src/header.o src/payload.o src/message.o src/parser.o src/serialiser.o src/test.o
