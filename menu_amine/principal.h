#ifndef PRINCIPAL_H_INCLUDED
#define PRINCIPAL_H_INCLUDED

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include "option.h"

// Function prototypes
void principal_menu(SDL_Surface* ecran, Mix_Music* musique, int* menustate, SDL_Surface* image);

#endif // PRINCIPAL_H_INCLUDED