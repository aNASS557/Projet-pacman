#include <stdio.h>
#include "labyrinthe.h"

int main() {
    charger_labyrinthe("labyrinthe.txt");

    // Affichage du labyrinthe pour vérification
    for (int y = 0; y < N_Y_TILES; y++) {
        for (int x = 0; x < N_X_TILES; x++) {
            switch (map[y][x]) {
                case WALL: printf("W"); break;
                case PATH: printf(" "); break;
                case PACMAN_START: printf("S"); break;
                case GHOST_START: printf("G"); break;
                default: printf("?"); break;
            }
        }
        printf("\n");
    }

    return 0;
}

