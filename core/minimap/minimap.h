#ifndef MINIMAP_H
#define MINIMAP_H

#include <SDL/SDL.h>

typedef struct {
    SDL_Surface *fond;
    SDL_Surface *joueurImage;
    SDL_Rect position;
    SDL_Rect joueurPosition;
} Minimap;

int initialiserMinimapAssets(Minimap *m, const char *minimapPlayerIconPath);
void MAJMinimap(SDL_Rect posJoueurAbs, Minimap *m, float redimensionnement);
void libererMinimapAssets(Minimap *m);

#endif
