#include "../include/minimap.h"
#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL_image.h>

int initialiserMinimapAssets(Minimap *m) {
    if (!m) return 0;

    // Load the minimap background image
    m->fond = IMG_Load("./assets/game/minimap3.png");
    if (!m->fond) {
        printf("Failed to load minimap background: %s\n", IMG_GetError());
        return 0;
    }

    // Set the position for the minimap background (top-left corner with some padding)
    m->position = (SDL_Rect){10, 10, m->fond->w, m->fond->h};
    m->joueurPosition = (SDL_Rect){0, 0, MINIMAP_PLAYER_WIDTH, MINIMAP_PLAYER_HEIGHT};
    return 1;
}

void MAJMinimap(SDL_Rect posJoueurAbs, Minimap *m, float redimensionnement) {
    if (!m || redimensionnement <= 0) return;

    float centerX = posJoueurAbs.x + posJoueurAbs.w / 2.0f;
    float centerY = posJoueurAbs.y + posJoueurAbs.h / 2.0f;

    // Adjust position relative to minimap background position
    m->joueurPosition.x = m->position.x + (int)(centerX * redimensionnement) - MINIMAP_PLAYER_WIDTH / 2;
    m->joueurPosition.y = m->position.y + (int)(centerY * redimensionnement) - MINIMAP_PLAYER_HEIGHT / 2;
}

void dessinerJoueurMinimap(SDL_Surface *screen, Minimap *m) {
    if (!m || !screen) return;
    
    // Draw the minimap background first
    SDL_BlitSurface(m->fond, NULL, screen, &m->position);
    
    // Draw the player marker
    SDL_Rect rect = m->joueurPosition;
    SDL_FillRect(screen, &rect, MINIMAP_PLAYER_COLOR);
}

void libererMinimapAssets(Minimap *m) {
    if (!m) return;
    if (m->fond) SDL_FreeSurface(m->fond);
    m->fond = NULL;
}
