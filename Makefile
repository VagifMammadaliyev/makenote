CFLAGS=-Wall -Wextra -Wpedantic
SOURCES = $(wildcard src/*.c)
OBJECTS = $(patsubst %.c,%.o,$(SOURCES))
BIN_NAME = makenote
DIR_NAME = ~/.$(BIN_NAME)

all: build directory $(OBJECTS)
	cc $(OBJECTS) -o bin/$(BIN_NAME)

debug: $(SOURCES)
	cc -g $(SOURCES)

build:
	-mkdir bin

directory:
	-mkdir $(DIR_NAME)

install: all directory
	install ./bin/$(BIN_NAME) /usr/local/bin/

uninstall: clean
	-rm /usr/local/bin/$(BIN_NAME)

clean:
	-rm $(OBJECTS)
	-rm -rf bin	
	-rm -rf *.out *.out.dSYM
	-rm temp/*.notebook
	-rm -rf $(DIR_NAME)
