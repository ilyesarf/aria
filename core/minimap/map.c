#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_map(Map *m, SDL_Surface *screen) {
    m->map = IMG_Load("mini_backgg.png");
    if (!m->map) {
        printf("Failed to load map: %s\n", SDL_GetError());
        exit(1);
    }
    m->pos_map.x = 1000;
    m->pos_map.y = 100;

    m->mini_perso = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_DOUBLEBUF, 10, 10, 32, 0, 0, 0, 0);
    if (!m->mini_perso) {
        printf("Failed to create mini_perso surface: %s\n", SDL_GetError());
        exit(1);
    }
    SDL_FillRect(m->mini_perso, NULL, SDL_MapRGB(screen->format, 0, 255, 0));

    m->perso_pos_map.x = 0;
    m->perso_pos_map.y = 0;
}

void update_map_position(Map *m, personne *p) {
    m->perso_pos_map.x = p->pos_perso.x / 10 + m->pos_map.x;
    m->perso_pos_map.y = p->pos_perso.y / 10 + 10 + m->pos_map.y;
}

void render_map(Map *m, SDL_Surface *screen) {
    SDL_BlitSurface(m->map, NULL, screen, &m->pos_map);
    SDL_BlitSurface(m->mini_perso, NULL, screen, &m->perso_pos_map);
}