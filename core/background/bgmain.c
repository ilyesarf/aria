#include "bgmain.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

// Camera follows the player, clamped to world bounds
void updateBackgroundCamera(Background *bg, const SDL_Rect *player, int screenWidth, int screenHeight, int margin) {
    int targetX = player->x + player->w / 2 - bg->camera.w / 2;
    int targetY = player->y + player->h / 2 - bg->camera.h / 2;

    if (targetX < 0) targetX = 0;
    if (targetY < 0) targetY = 0;
    if (targetX > bg->world_width - bg->camera.w)
        targetX = bg->world_width - bg->camera.w;
    if (targetY > bg->world_height - bg->camera.h)
        targetY = bg->world_height - bg->camera.h;

    bg->camera.x = targetX;
    bg->camera.y = targetY;
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Surface *screen = SDL_SetVideoMode(800, 600, 32, SDL_SWSURFACE);
    IMG_Init(IMG_INIT_PNG);

    Background bg;
    //time_t start_time = time(NULL);

    // Initialize background with a PNG file
    init_background(&bg, "./assets/game/background.png", 1);

    int running = 1;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        // Clear the screen
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));

        // Display the background
        display_background(&bg, screen);

        // Update the screen
        SDL_Flip(screen);
        SDL_Delay(16); // ~60 FPS
    }

    // Free resources
    free_background(&bg);
    IMG_Quit();
    SDL_Quit();

    return 0;
}