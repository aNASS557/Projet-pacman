#include <stdio.h>
#include "labyrinthe.h"

int map[N_Y_TILES][N_X_TILES];

void charger_labyrinthe(const char *nom_fichier) {
    FILE *fichier = fopen(nom_fichier, "r");
    if (!fichier) {
        fprintf(stderr, "Impossible d'ouvrir le fichier %s. Utilisation du labyrinthe par défaut.\n", nom_fichier);
        return;
    }

    char ligne[N_X_TILES + 2]; // +2 pour '\n' et '\0'
    int y = 0;

    while (fgets(ligne, sizeof(ligne), fichier) && y < N_Y_TILES) {
        for (int x = 0; x < N_X_TILES && ligne[x] != '\n' && ligne[x] != '\0'; x++) {
            switch (ligne[x]) {
                case 'W':
                    map[y][x] = WALL;
                    break;
                case ' ':
                    map[y][x] = PATH;
                    break;
                case 'S':
                    map[y][x] = PACMAN_START;
                    break;
                case 'G':
                    map[y][x] = GHOST_START;
                    break;
                default:
                    map[y][x] = PATH;
                    break;
            }
        }
        y++;
    }

    fclose(fichier);
}
