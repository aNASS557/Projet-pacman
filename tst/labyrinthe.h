#ifndef LABYRINTHE_H
#define LABYRINTHE_H

#define N_X_TILES 10
#define N_Y_TILES 10

// Définir les types de cases
#define WALL 1
#define PATH 0
#define PACMAN_START 2
#define GHOST_START 3

extern int map[N_Y_TILES][N_X_TILES];

void charger_labyrinthe(const char *nom_fichier);

#endif

