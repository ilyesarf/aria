#include "enemy.h"
#include <SDL/SDL.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Surface* screen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);

    SDL_Surface* enemy_sprite = SDL_LoadBMP("enemy.bmp");
    SDL_SetColorKey(enemy_sprite, SDL_SRCCOLORKEY, SDL_MapRGB(enemy_sprite->format, 255, 0, 255));

    Enemy enemy1;
    init_enemy(&enemy1, 100, 0, 0, enemy_sprite);

    int running = 1;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        move_enemy_randomly(&enemy1, 1);
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
        display_enemy(&enemy1, screen);
        SDL_Flip(screen);

        SDL_Delay(30);
    }

    SDL_FreeSurface(enemy_sprite);
    SDL_Quit();

    return 0;
}