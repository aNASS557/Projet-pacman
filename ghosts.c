#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "ghosts.h"

struct ghost* tableau_vide(int nb_ghosts){
    return malloc(nb_ghosts*sizeof(struct ghost));  // génrer tableau qui a nb_ghost
}

//fonction pour initialiser un fantome
struct ghost initialize_ghost(int identifiant, SDL_Rect position){
    int direction=SDLK_LEFT;
    struct ghost g = {identifiant,direction, position}; // initialise la structure.
    return g;
}

// creer fx pour free tableau créé avec malloc
void liberation(struct ghost* tablo){
    free(tablo);
}

void enregistrer(struct ghost* tablo, struct ghost g){
    tablo[g.identifiant]=g; 
}

SDL_Texture* texture_fantome(SDL_Renderer* renderer, int identifiant){
    // en fonction du type de fantome on renvoie la strucutre correspondante
    switch (identifiant) {
        case 0:
            return IMG_LoadTexture(renderer, "images/ghost-blue-left.png");
        case 1:
            return IMG_LoadTexture(renderer, "images/ghost-orange-right.png");  
        case 2:
            return IMG_LoadTexture(renderer, "images/ghost-pink-left.png");
        case 3:
            return IMG_LoadTexture(renderer, "images/ghost-red-right.png");
    }
    return NULL;
}

SDL_Texture* yeux_fantome(SDL_Renderer* renderer, int direction){
    // en fonction de la direction du fantome on renvoie la strucutre 
    switch (direction)
    {
        case SDLK_UP:
            return IMG_LoadTexture(renderer, "images/ghost-eyes-up.png");

        case SDLK_DOWN:
            return IMG_LoadTexture(renderer, "images/ghost-eyes-down.png");

        case SDLK_LEFT:
            return IMG_LoadTexture(renderer, "images/ghost-eyes-left.png");

        case SDLK_RIGHT:
            return IMG_LoadTexture(renderer, "images/ghost-eyes-right.png");

    }  
    return NULL;
}

