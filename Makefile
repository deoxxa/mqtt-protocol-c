CFLAGS += -Wall -Werror -pedantic

all: bin/test

clean:
	rm src/*.o bin/*

src/errors.o: src/errors.c
src/parser.o: src/parser.c
src/test.o: src/test.c

bin/test: src/errors.o src/parser.o src/test.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o bin/test src/errors.o src/parser.o src/test.o
