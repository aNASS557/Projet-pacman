struct ghost {
    int identifiant;
    int direction;
    SDL_Rect position; // stocker position ghost
};

struct ghost* tableau_vide(int nb_ghosts);
struct ghost initialize_ghost(int identifiant_fantome, SDL_Rect position);
void liberation(struct ghost* tablo);
void enregistrer(struct ghost* tablo, struct ghost g);
SDL_Texture* texture_fantome(SDL_Renderer* renderer, int identifiant_fantome);
SDL_Texture* yeux_fantome(SDL_Renderer* renderer, int direction);
