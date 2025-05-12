#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "map.h"
#include "time.h"
#include "pres.h"

int main() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        printf("Failed to initialize SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Surface *screen = SDL_SetVideoMode(1500, 1000, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (!screen) {
        printf("Failed to set video mode: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Surface *background = SDL_LoadBMP("backgg1.bmp");
    if (!background) {
        printf("Failed to load background: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Rect background_pos = {0, 0};

    Time timer;
    Map map;
    personne player;

    inittemps(&timer);
    init_map(&map, screen);
    initialiserperso(&player);

    int running = 1;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
            deplacer_perso(&player, event);
        }

        update_time(&timer);
        update_map_position(&map, &player);

        SDL_BlitSurface(background, NULL, screen, &background_pos);
        displaytime(timer, screen);
        render_map(&map, screen);
        afficher_perso(screen, player);

        SDL_Flip(screen);
        SDL_Delay(100);
    }

    freeTexttime(timer.temps);
    free_perso(player);
    SDL_FreeSurface(background);
    SDL_Quit();

    return 0;
}