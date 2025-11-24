CC=gcc
CFLAGS=-Wall -Werror -std=c99 $(shell pkg-config --cflags sdl2 SDL2_image)
LDFLAGS=$(shell pkg-config --libs sdl2 SDL2_image)
LDLIBS=$(shell pkg-config --libs sdl2 SDL2_image)

EXEC=pacman
SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS) $(LDLIBS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -f $(OBJ) $(EXEC)

