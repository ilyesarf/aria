#ifndef MENUJOUEUR_H
#define MENUJOUEUR_H


#include <stdio.h>

#include "../header.h"

void initMenuJoueur(Menu *menus);
void initMenuJoueurButtons(Button *buttons);
void renderMenuJoueur(SDL_Surface *background, SDL_Surface *butImage, SDL_Surface *screen, TTF_Font *font, SDL_Color textColor, Button *buttons, int n_btns);
void handleEventJoueurMenu(int *menuState, Save save, SDL_Event event, Button *buttons, int n_btns, Mix_Chunk *hoverSound);
void cleanupMenuJoueur(Menu *menu) ;

#endif