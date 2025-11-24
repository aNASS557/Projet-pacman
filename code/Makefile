CC=gcc
CFLAGS=-Wall -Werror -std=c99
NOM_PROGRAMME = pacman /*pacman est la cible*/
LDLIBS= $(shell pkg-config --libs sdl2 SDL2_image)
SRC =  pacman.c
OBJ =  $(SRC: .c=.o)

all: pacman

pacman : pacman.o
	$(CC) -o $(CFLAGS) -o $(NOM_PROGRAMME) $(OBJ) 

pacman.o : pacman.c 
	$(CC) -o $@ -c $< $(CFLAGS)


clean: rm -f $(OBJ) $(NOM_PROGRAMME)