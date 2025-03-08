#ifndef MENUJOUEUR_H
#define MENUJOUEUR_H

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>

#include "../header.h"

void initMenuJoueur(Menu *menus);
void initMenuJoueurButtons(Button *buttons);
void renderMenuJoueur(SDL_Surface *background, SDL_Surface *screen, TTF_Font *font, SDL_Color textColor, Button *buttons, int n_btns);
void handleEventJoueurMenu(int *menuState, SDL_Event event, Button *buttons, int n_btns, Mix_Chunk *hoverSound);

#endif