#include <stdio.h>
#include <SDL2/SDL.h>
#include "./constants.h"

// Définition de la structure pour le joueur
struct Player {
    float x;
    float y;
    float width;
    float height;
};

// Variables SDL globales
int game_is_running = FALSE;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
struct Player player;

float player_speed_x = 0.0f;
float player_speed_y = 0.0f;

int last_frame_time = 0;

// Initialisation de la fenêtre
int initialize_window(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initializing SDL.\n");
        return FALSE;
    }
    // Caractéristiques de la fenêtre
    window = SDL_CreateWindow("Super Jeu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
    if (!window) {
        fprintf(stderr, "Error creating SDL Window.\n");
        return FALSE;
    }
    // Caractéristiques du rendu 
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer) {
        fprintf(stderr, "Error creating SDL Renderer.\n");
        return FALSE;
    }

    return TRUE;
}

// Initialisation du jeu
void setup(void) {
    player.x = 20;
    player.y = 20;
    player.width = 40;
    player.height = 40;
}

// Gestion des événements
void process_input(void) {
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type) {
        case SDL_QUIT:
            game_is_running = FALSE;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    game_is_running = FALSE;
                    break;
                case SDLK_UP:
                    player_speed_y = -PLAYER_SPEED_Y;
                    break;
                case SDLK_DOWN:
                    player_speed_y = PLAYER_SPEED_Y;
                    break;
                case SDLK_LEFT:
                    player_speed_x = -PLAYER_SPEED_X;
                    break;
                case SDLK_RIGHT:
                    player_speed_x = PLAYER_SPEED_X;
                    break;
            }
            break;
        case SDL_KEYUP:
            switch (event.key.keysym.sym) {
                case SDLK_UP:
                case SDLK_DOWN:
                    player_speed_y = 0.0f;
                    break;
                case SDLK_LEFT:
                case SDLK_RIGHT:
                    player_speed_x = 0.0f;
                    break;
            }
            break;
    }
}

// Mise à jour des evenements : position du jour , vitesse ,ect...
void update(void) {
    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);

    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME)
        SDL_Delay(time_to_wait);

    float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;

    last_frame_time = SDL_GetTicks();

    player.x += player_speed_x * delta_time;
    player.y += player_speed_y * delta_time;
}

// Rendu
void render(void) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 220);
    SDL_RenderClear(renderer);

    SDL_Rect player_rect = { 
        (int)player.x,
        (int)player.y,
        (int)player.width,
        (int)player.height
    };
    SDL_SetRenderDrawColor(renderer, 255, 255, 100, 255);
    SDL_RenderFillRect(renderer, &player_rect);

    SDL_RenderPresent(renderer);
}

// Destruction de la fenêtre
void destroy_window(void) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(void) {
    // Initialisation de la fenêtre et du jeu
    game_is_running = initialize_window();
    setup();

    // Boucle principale du jeu
    while (game_is_running) {
        process_input();
        update();
        render();
    }

    // Destruction de la fenêtre à la fin du jeu
    destroy_window();
    return (0);
}
