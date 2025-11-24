#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define RENDER_SCALE 0.7
#define N_X_TILES 28
#define N_Y_TILES 29
#define PACMAN_SIZE 32

enum tile_type_e {
    WALL = 0,
    PATH,
    PACMAN_START,
};

int can_move(enum tile_type_e map[N_Y_TILES][N_X_TILES], int x, int y) {
    return (x >= 0 && x < N_X_TILES && y >= 0 && y < N_Y_TILES && map[y][x] != WALL);
}

int main() {
    enum tile_type_e map[N_Y_TILES][N_X_TILES] = { /* Définition du labyrinthe */ };
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *screen = SDL_CreateWindow("Pacman", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED);
    
    SDL_Rect pacman_position = { .w = PACMAN_SIZE, .h = PACMAN_SIZE };
    int pac_x = 1, pac_y = 1; // Position en tuiles
    int move_direction = 0, desired_direction = 0;

    int is_running = 1;
    while (is_running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) is_running = 0;
            else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_q: is_running = 0; break;
                    case SDLK_UP: desired_direction = SDLK_UP; break;
                    case SDLK_DOWN: desired_direction = SDLK_DOWN; break;
                    case SDLK_LEFT: desired_direction = SDLK_LEFT; break;
                    case SDLK_RIGHT: desired_direction = SDLK_RIGHT; break;
                }
            }
        }
        
        int next_x = pac_x, next_y = pac_y;
        switch (desired_direction) {
            case SDLK_UP: next_y--; break;
            case SDLK_DOWN: next_y++; break;
            case SDLK_LEFT: next_x--; break;
            case SDLK_RIGHT: next_x++; break;
        }
        
        if (can_move(map, next_x, next_y)) {
            move_direction = desired_direction;
            pac_x = next_x;
            pac_y = next_y;
        }
        
        pacman_position.x = pac_x * PACMAN_SIZE;
        pacman_position.y = pac_y * PACMAN_SIZE;
        
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, NULL, NULL, &pacman_position);
        SDL_RenderPresent(renderer);
        SDL_Delay(100);
    }
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(screen);
    SDL_Quit();
    return EXIT_SUCCESS;
}
