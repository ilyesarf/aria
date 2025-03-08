#ifndef MENUOPTION_H
#define MENUOPTION_H

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>

#include "../header.h"

void initMenuOption(Menu *menus);
void initMenuOptionButtons(Button *buttons);
void renderMenuOption(SDL_Surface *background, SDL_Surface *screen, TTF_Font *font, SDL_Color textColor, Button *buttons, int n_btns);
void handleEventOptionMenu(int *menuState, SDL_Event event, Button *buttons, int n_btns, Mix_Chunk *hoverSound);

#endif