#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "pres.h"

typedef struct {
    SDL_Surface *map;
    SDL_Rect pos_map;
    SDL_Surface *mini_perso;
    SDL_Rect perso_pos_map;
} Map;

void init_map(Map *m, SDL_Surface *screen);
void update_map_position(Map *m, personne *p);
void render_map(Map *m, SDL_Surface *screen);


#endif
