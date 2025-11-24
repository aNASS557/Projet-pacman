#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include "ghosts.h"

#define RENDER_SCALE 0.7
#define N_X_TILES 28
#define N_Y_TILES 29
#define PACMAN_SIZE 32
#define TILE_SIZE 40
#define NB_FANTOME 4

enum tile_type_e {
    WALL = 0,
    PATH,
    PACMAN_START,
};

void set_window_position_coords(SDL_Rect* window_rect) {
    int mouse_x, mouse_y;
    SDL_GetGlobalMouseState(&mouse_x, &mouse_y);
    int nb_displays = SDL_GetNumVideoDisplays();
    SDL_Rect display_bounds;

    for (int i = 0; i < nb_displays; i++) {
        SDL_GetDisplayBounds(i, &display_bounds);
        if (mouse_x >= display_bounds.x && mouse_x < (display_bounds.x + display_bounds.w) &&
                mouse_y >= display_bounds.y && mouse_y < (display_bounds.y + display_bounds.h)) {
            break;
        }
    }

    window_rect->x = display_bounds.x + (display_bounds.w - window_rect->w * RENDER_SCALE) / 2;
    window_rect->y = display_bounds.y + (display_bounds.h - window_rect->h * RENDER_SCALE) / 2;
}

enum tile_type_e map[N_Y_TILES][N_X_TILES] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0},
    {0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0},
    {0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0},
    {0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 'G', 1, 1, 1, 1, 'G', 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 'G', 1, 1, 1, 1, 'G', 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0},
    {0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0},
    {0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0},
    {0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

int peut_bouger(int direction, SDL_Rect position) {
    int position_x = position.x / TILE_SIZE;
    int position_y = position.y / TILE_SIZE;
    int num_tile = 0;

    switch (direction) {
        case SDLK_UP:
            num_tile = map[position_y - 1][position_x];
            break;
        case SDLK_DOWN:
            num_tile = map[position_y + 1][position_x];
            break;
        case SDLK_LEFT:
            num_tile = map[position_y][position_x - 1];
            break;
        case SDLK_RIGHT:
            num_tile = map[position_y][position_x + 1];
            break;
        default:
            return 0;
    }
    return num_tile;
}

int dir_oppose(int direction) {
    switch (direction) {
        case SDLK_UP:
            return SDLK_DOWN;
        case SDLK_DOWN:
            return SDLK_UP;
        case SDLK_LEFT:
            return SDLK_RIGHT;
        case SDLK_RIGHT:
            return SDLK_LEFT;
        default:
            return 0;
    }
}

int main() {
    struct ghost* stocker_fantomes = tableau_vide(NB_FANTOME);
    int nombre_fantomes_crees = 0;
    int ret;
    int is_running = 1;
    const int tile_size = PACMAN_SIZE + 8;
    int nb_pac_gomme = 0;
    int start_vulnerable = 0;
    int vers_pacman = 0;
    int **liste_noeuds = NULL;
    int nb_noeuds = 0;
    int move_direction = 0;
    int move_direction_choisie = 0;
    int id_direction = 0;
    int n = 0;
    int nb_points = 0;
    int liste_directions[4] = {SDLK_UP, SDLK_LEFT, SDLK_DOWN, SDLK_RIGHT};
    int fantome_mange[4] = {0, 0, 0, 0};
    int nb_g_mange = 0;
    srand(time(NULL));

    SDL_Rect window_rect = {
        .w = N_X_TILES * tile_size,
        .h = N_Y_TILES * tile_size,
        .x = SDL_WINDOWPOS_UNDEFINED,
        .y = SDL_WINDOWPOS_UNDEFINED
    };

    ret = SDL_Init(SDL_INIT_VIDEO);
    if (ret != 0) {
        fprintf(stderr, "Could not init SDL: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    set_window_position_coords(&window_rect);
    SDL_Window *screen = SDL_CreateWindow(
                               "Pacman",
                               window_rect.x, window_rect.y,
                               window_rect.w * RENDER_SCALE, window_rect.h * RENDER_SCALE,
                               0
                           );
    if (!screen) {
        fprintf(stderr, "Could not create SDL screen: %s\n", SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_SOFTWARE);
    if (!renderer) {
        fprintf(stderr, "Could not create SDL renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(screen);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    ret = SDL_RenderSetScale(renderer, RENDER_SCALE, RENDER_SCALE);
    if (ret < 0) {
        fprintf(stderr, "Could not scale SDL renderer: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(screen);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_Rect pacman_position = {
        .w = PACMAN_SIZE,
        .h = PACMAN_SIZE
    };

    SDL_Rect map_position = {
        .x = 0,
        .y = 0,
        .w = window_rect.w,
        .h = window_rect.h
    };

    SDL_Texture* map_texture = SDL_CreateTexture(
                                   renderer,
                                   SDL_PIXELFORMAT_RGBA8888,
                                   SDL_TEXTUREACCESS_TARGET,
                                   window_rect.w,
                                   window_rect.h
                               );
    if (!map_texture) {
        fprintf(stderr, "Could not create SDL texture: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(screen);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_SetRenderTarget(renderer, map_texture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (int y = 0; y < N_Y_TILES; y++) {
        for (int x = 0; x < N_X_TILES; x++) {
            if (map[y][x] == WALL) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                SDL_Rect rectangle = {
                    .x = x * tile_size,
                    .y = y * tile_size,
                    .w = tile_size,
                    .h = tile_size
                };
                SDL_RenderFillRect(renderer, &rectangle);
            } else if (map[y][x] == PACMAN_START) {
                pacman_position.x = x * tile_size + (tile_size - PACMAN_SIZE) / 2;
                pacman_position.y = y * tile_size + (tile_size - PACMAN_SIZE) / 2;
                map[y][x] = PATH;
            } else if (map[y][x] == 'G') {
                SDL_Rect fantome = {
                    .w = PACMAN_SIZE,
                    .h = PACMAN_SIZE,
                    .x = x * tile_size + (tile_size - PACMAN_SIZE) / 2,
                    .y = y * tile_size + (tile_size - PACMAN_SIZE) / 2
                };
                enregistrer(stocker_fantomes, initialize_ghost(nombre_fantomes_crees, fantome));
                nombre_fantomes_crees++;
                map[y][x] = PATH;
            } else if (map[y][x] == PATH) {
                if ((map[y][x - 1] && (map[y + 1][x] || map[y - 1][x])) || (map[y][x + 1] && (map[y + 1][x] || map[y - 1][x]))) {
                    liste_noeuds = realloc(liste_noeuds, (n + 1) * sizeof(int *));
                    if (liste_noeuds == NULL) {
                        fprintf(stderr, "Erreur realloc liste_noeuds\n");
                        for (int i = 0; i < n; i++) {
                            free(liste_noeuds[i]);
                        }
                        free(liste_noeuds);
                        SDL_DestroyTexture(map_texture);
                        SDL_DestroyRenderer(renderer);
                        SDL_DestroyWindow(screen);
                        SDL_Quit();
                        return EXIT_FAILURE;
                    }

                    liste_noeuds[n] = malloc(2 * sizeof(int));
                    if (liste_noeuds[n] == NULL) {
                        fprintf(stderr, "Erreur malloc liste_noeuds[n]\n");
                        for (int i = 0; i < n; i++) {
                            free(liste_noeuds[i]);
                        }
                        free(liste_noeuds);
                        SDL_DestroyTexture(map_texture);
                        SDL_DestroyRenderer(renderer);
                        SDL_DestroyWindow(screen);
                        SDL_Quit();
                        return EXIT_FAILURE;
                    }

                    liste_noeuds[n][0] = x;
                    liste_noeuds[n][1] = y;
                    nb_noeuds++;
                }

                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
                SDL_Rect pac_gomme = {
                    .w = 10,
                    .h = 10,
                    .x = x * tile_size + (tile_size - 10) / 2,
                    .y = y * tile_size + (tile_size - 10) / 2
                };
                SDL_RenderFillRect(renderer, &pac_gomme);
                nb_pac_gomme += 1;
                map[y][x] = 3;
            }
        }
    }

    SDL_SetRenderTarget(renderer, NULL);

    SDL_Texture* pacman_texture = IMG_LoadTexture(renderer, "images/pacman-left.png");
    if (!pacman_texture) {
        fprintf(stderr, "Could not load image: %s\n", SDL_GetError());
        for (int i = 0; i < nb_noeuds; i++) {
            free(liste_noeuds[i]);
        }
        free(liste_noeuds);
        SDL_DestroyTexture(map_texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(screen);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_Texture* texture[4] = {
        texture_fantome(renderer, stocker_fantomes[0].identifiant),
        texture_fantome(renderer, stocker_fantomes[1].identifiant),
        texture_fantome(renderer, stocker_fantomes[2].identifiant),
        texture_fantome(renderer, stocker_fantomes[3].identifiant)
    };

    SDL_Texture* texture_oeuil[4] = {
        yeux_fantome(renderer, SDLK_UP),
        yeux_fantome(renderer, SDLK_DOWN),
        yeux_fantome(renderer, SDLK_LEFT),
        yeux_fantome(renderer, SDLK_RIGHT)
    };

    SDL_Texture* texture_vulnerable = texture_fantome(renderer, stocker_fantomes[4].identifiant);

    SDL_Event event;

    while (is_running) {
        SDL_PollEvent(&event);

        switch (event.type) {
            case SDL_QUIT:
                is_running = 0;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_q:
                        is_running = 0;
                        break;
                    case SDLK_UP:
                    case SDLK_DOWN:
                    case SDLK_LEFT:
                    case SDLK_RIGHT:
                        move_direction_choisie = event.key.keysym.sym;
                        break;
                }
                break;
        }

        if (pacman_position.x % 40 == 4 && pacman_position.y % 40 == 4) {
            if (peut_bouger(move_direction_choisie, pacman_position)) {
                move_direction = move_direction_choisie;
                SDL_Texture* new_pacman_texture = NULL;
                switch (move_direction) {
                    case SDLK_UP:
                        new_pacman_texture = IMG_LoadTexture(renderer, "images/pacman-up.png");
                        break;
                    case SDLK_DOWN:
                        new_pacman_texture = IMG_LoadTexture(renderer, "images/pacman-down.png");
                        break;
                    case SDLK_LEFT:
                        new_pacman_texture = IMG_LoadTexture(renderer, "images/pacman-left.png");
                        break;
                    case SDLK_RIGHT:
                        new_pacman_texture = IMG_LoadTexture(renderer, "images/pacman-right.png");
                        break;
                }
                if (!new_pacman_texture) {
                    fprintf(stderr, "Could not load image: %s\n", SDL_GetError());
                    for (int i = 0; i < nb_noeuds; i++) {
                        free(liste_noeuds[i]);
                    }
                    free(liste_noeuds);
                    SDL_DestroyTexture(map_texture);
		    SDL_DestroyTexture(pacman_texture);
                    for (int i = 0; i < 4; i++) {
                        SDL_DestroyTexture(texture[i]);
                        SDL_DestroyTexture(texture_oeuil[i]);
                    }
                    SDL_DestroyTexture(texture_vulnerable);
                    SDL_DestroyRenderer(renderer);
                    SDL_DestroyWindow(screen);
                    SDL_Quit();
		    free(stocker_fantomes);
                    return EXIT_FAILURE;
                }
                SDL_DestroyTexture(pacman_texture);
                pacman_texture = new_pacman_texture;
            }
        }

        if (peut_bouger(move_direction, pacman_position)) {
            switch (move_direction) {
                case SDLK_UP:
                    pacman_position.y--;
                    break;
                case SDLK_DOWN:
                    pacman_position.y++;
                    break;
                case SDLK_LEFT:
                    pacman_position.x--;
                    break;
                case SDLK_RIGHT:
                    pacman_position.x++;
                    break;
            }

            if (map[pacman_position.y / TILE_SIZE][pacman_position.x / TILE_SIZE] == 3) {
                SDL_SetRenderTarget(renderer, map_texture);
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

                SDL_Rect pac_gomme_efface = {
                    .w = PACMAN_SIZE,
                    .h = PACMAN_SIZE,
                    .x = pacman_position.x,
                    .y = pacman_position.y
                };
                SDL_RenderFillRect(renderer, &pac_gomme_efface);
                SDL_SetRenderTarget(renderer, NULL);
                nb_points++;
                map[pacman_position.y / 40][pacman_position.x / 40] = PATH;
            }
        } else {
            switch (move_direction) {
                case SDLK_UP:
                    pacman_position.y--;
                    break;
                case SDLK_DOWN:
                    pacman_position.y++;
                    break;
                case SDLK_LEFT:
                    pacman_position.x--;
                    break;
                case SDLK_RIGHT:
                    pacman_position.x++;
                    break;
            }
        }

        if (move_direction != 0) {
            for (int i = 0; i < nombre_fantomes_crees; i++) {
                if (fantome_mange[i] == 0) {
                    SDL_Rect position_fantome = stocker_fantomes[i].position;

                    if (SDL_HasIntersection(&pacman_position, &position_fantome)) {
                        if (start_vulnerable) {
                            nb_g_mange += 1;
                            fantome_mange[i] = 1;
                        } else {
                            printf("Perdu !\n");
                            is_running = 0;
                        }
                    }
                    if (SDL_GetTicks() - start_vulnerable > 5000) {
                        start_vulnerable = 0;
                    }

                    if (start_vulnerable == 0) {
                        if (abs(position_fantome.x - pacman_position.x) > abs(position_fantome.y - pacman_position.y)) {
                            if (position_fantome.x >= pacman_position.x) {
                                vers_pacman = SDLK_LEFT;
                            } else {
                                vers_pacman = SDLK_RIGHT;
                            }
                        } else {
                            if (position_fantome.y >= pacman_position.y) {
                                vers_pacman = SDLK_UP;
                            } else {
                                vers_pacman = SDLK_DOWN;
                            }
                        }
                        if (start_vulnerable == 0 && vers_pacman != dir_oppose(stocker_fantomes[i].direction) && peut_bouger(vers_pacman, position_fantome)) {
                            stocker_fantomes[i].direction = vers_pacman;
                        } else {
                            int tmp = 1;
                            while (tmp) {
                                int rdm = rand() % 4;
                                if (peut_bouger(liste_directions[rdm], position_fantome) &&
                                        liste_directions[rdm] != dir_oppose(stocker_fantomes[i].direction)) {
                                    stocker_fantomes[i].direction = liste_directions[rdm];
                                    tmp = 0;
                                }
                            }
                        }

                        switch (stocker_fantomes[i].direction) {
                            case SDLK_UP:
                                id_direction = 0;
                                stocker_fantomes[i].position.y--;
                                break;
                            case SDLK_DOWN:
                                id_direction = 1;
                                stocker_fantomes[i].position.y++;
                                break;
                            case SDLK_LEFT:
                                id_direction = 2;
                                stocker_fantomes[i].position.x--;
                                break;
                            case SDLK_RIGHT:
                                id_direction = 3;
                                stocker_fantomes[i].position.x++;
                                break;
                        }
                    }
                }
            }
        }

        if (nb_points == 50 || nb_points == 100 || nb_points == 150 || nb_points == 250) {
            start_vulnerable = SDL_GetTicks();
        }

        if (nb_points == nb_pac_gomme) {
            printf("Gagné !\n");
            is_running = 0;
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, map_texture, NULL, &map_position);
        SDL_RenderCopy(renderer, pacman_texture, NULL, &pacman_position);

        for (int i = 0; i < nombre_fantomes_crees; i++) {
            if (fantome_mange[i] == 0) {
                if (start_vulnerable) {
                    SDL_RenderCopy(renderer, texture_vulnerable, NULL, &stocker_fantomes[i].position);
                } else {
                    SDL_RenderCopy(renderer, texture[i], NULL, &stocker_fantomes[i].position);
                }
            }
        }

        SDL_RenderPresent(renderer);

        SDL_Delay(10);
    }

    // Free memory before exiting
    for (int i = 0; i < nb_noeuds; i++) {
        free(liste_noeuds[i]);
    }
    free(liste_noeuds);
    SDL_DestroyTexture(map_texture);
    SDL_DestroyTexture(pacman_texture);
    for (int i = 0; i < 4; i++) {
        SDL_DestroyTexture(texture[i]);
        SDL_DestroyTexture(texture_oeuil[i]);
    }
    SDL_DestroyTexture(texture_vulnerable);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(screen);
    SDL_Quit();
    free(stocker_fantomes);
    return 0;
}
