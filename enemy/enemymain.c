#include "enemy.h"
#include <SDL/SDL.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Surface* screen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);

    SDL_Surface* frames[4];
    frames[0] = SDL_LoadBMP("shadowf1t.bmp");
    frames[1] = SDL_LoadBMP("shadowf2t.bmp");
    frames[2] = SDL_LoadBMP("shadowf3t.bmp");
    frames[3] = SDL_LoadBMP("shadowf4t.bmp");

    for (int i = 0; i < 4; i++) {
        SDL_SetColorKey(frames[i], SDL_SRCCOLORKEY, SDL_MapRGB(frames[i]->format, 255, 0, 255));
    }

    Enemy enemy1;
    init_enemy(&enemy1, 100, 0, 0, frames);

    int running = 1;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        move_enemy_randomly(&enemy1, 1);
        animate_enemy_move(&enemy1);

        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
        display_enemy(&enemy1, screen);
        SDL_Flip(screen);

        SDL_Delay(100); // Adjust the delay for animation speed
    }

    for (int i = 0; i < 4; i++) {
        SDL_FreeSurface(frames[i]);
    }
    SDL_Quit();

    return 0;
}