#ifndef MENUOPTION_H
#define MENUOPTION_H


#include <stdio.h>

#include "../header.h"

void initMenuOption(Menu *menus);
void initMenuOptionButtons(Button *buttons);
void renderMenuOption(SDL_Surface *background, SDL_Surface *screen, TTF_Font *font, SDL_Color textColor, Button *buttons, int n_btns);
void handleEventOptionMenu(int *menuState, SDL_Event event, Button *buttons, int n_btns, Mix_Chunk *hoverSound);
void cleanupMenuOption(Menu *menu) ;
#endif