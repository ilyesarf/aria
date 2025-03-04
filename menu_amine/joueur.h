#ifndef JOUEUR_H_INCLUDED
#define JOUEUR_H_INCLUDED

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>

// Function prototypes
void joueur(SDL_Surface* ecran, SDL_Surface* background2, Mix_Music* music, TTF_Font* font, SDL_Color textColor, int* menustate);

#endif // JOUEUR_H_INCLUDED