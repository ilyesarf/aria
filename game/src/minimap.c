#include "../include/minimap.h"
#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL_image.h>

int initialiserMinimapAssets(Minimap *m) {
    if (!m) return 0;

    m->position = (SDL_Rect){0, 0, 0, 0};
    m->joueurPosition = (SDL_Rect){0, 0, MINIMAP_PLAYER_WIDTH, MINIMAP_PLAYER_HEIGHT};
    return 1;
}

void MAJMinimap(SDL_Rect posJoueurAbs, Minimap *m, float redimensionnement) {
    if (!m || redimensionnement <= 0) return;

    float centerX = posJoueurAbs.x + posJoueurAbs.w / 2.0f;
    float centerY = posJoueurAbs.y + posJoueurAbs.h / 2.0f;

    m->joueurPosition.x = (int)(centerX * redimensionnement) - MINIMAP_PLAYER_WIDTH / 2;
    m->joueurPosition.y = (int)(centerY * redimensionnement) - MINIMAP_PLAYER_HEIGHT / 2;
}

void dessinerJoueurMinimap(SDL_Surface *screen, Minimap *m) {
    if (!m || !screen) return;
    
    SDL_Rect rect = m->joueurPosition;
    SDL_FillRect(screen, &rect, MINIMAP_PLAYER_COLOR);
}

void libererMinimapAssets(Minimap *m) {
    if (!m) return;
    if (m->fond) SDL_FreeSurface(m->fond);
}
