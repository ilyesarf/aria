#include "minimap.h"
#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL_image.h>

int initialiserMinimapAssets(Minimap *m, const char *minimapPlayerIconPath) {
    if (!m || !minimapPlayerIconPath) return 0;

    m->joueurImage = IMG_Load(minimapPlayerIconPath);
    if (!m->joueurImage) {
        fprintf(stderr, "Erreur chargement joueur minimap : %s\n", IMG_GetError());
        return 0;
    }

    m->position = (SDL_Rect){0, 0, 0, 0};
    m->joueurPosition = (SDL_Rect){0, 0, 0, 0};
    return 1;
}

void MAJMinimap(SDL_Rect posJoueurAbs, Minimap *m, float redimensionnement) {
    if (!m || !m->joueurImage || redimensionnement <= 0) return;

    float centerX = posJoueurAbs.x + posJoueurAbs.w / 2.0f;
    float centerY = posJoueurAbs.y + posJoueurAbs.h / 2.0f;

    m->joueurPosition.x = (int)(centerX * redimensionnement) - m->joueurImage->w / 2;
    m->joueurPosition.y = (int)(centerY * redimensionnement) - m->joueurImage->h / 2;
    m->joueurPosition.w = m->joueurImage->w;
    m->joueurPosition.h = m->joueurImage->h;
}

void libererMinimapAssets(Minimap *m) {
    if (!m) return;
    if (m->joueurImage) SDL_FreeSurface(m->joueurImage);
    if (m->fond) SDL_FreeSurface(m->fond);
}
