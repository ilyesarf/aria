#include "background.h"
#include <time.h>

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Surface *screen = SDL_SetVideoMode(800, 600, 32, SDL_SWSURFACE);
    IMG_Init(IMG_INIT_PNG);

    Background bg;
    time_t start_time = time(NULL);

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