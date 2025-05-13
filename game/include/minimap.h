#ifndef MINIMAP_H
#define MINIMAP_H

#include <SDL/SDL.h>

#define MINIMAP_PLAYER_WIDTH 10
#define MINIMAP_PLAYER_HEIGHT 10
#define MINIMAP_PLAYER_COLOR 0x00FF00 // Green

typedef struct {
    SDL_Surface *fond;
    SDL_Rect position;
    SDL_Rect joueurPosition;
} Minimap;

int initialiserMinimapAssets(Minimap *m);
void MAJMinimap(SDL_Rect posJoueurAbs, Minimap *m, float redimensionnement);
void libererMinimapAssets(Minimap *m);
void dessinerJoueurMinimap(SDL_Surface *screen, Minimap *m);

#endif
