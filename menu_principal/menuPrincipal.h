#ifndef MENUPRINCIPAL_H
#define MENUPRINCIPAL_H

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>

#include "../header.h"

void initMenuPrincipal(Menu *menus);
void initMenuPrincipalButtons(Button *buttons);
void renderMenuPrincipal(SDL_Surface *background, SDL_Surface *screen, TTF_Font *font, SDL_Color textColor, Button *buttons, int n_btns);
void handleEventPrincipalMenu(int *menuState, SDL_Event event, Button *buttons, int n_btns, Mix_Chunk *hoverSound);

#endif