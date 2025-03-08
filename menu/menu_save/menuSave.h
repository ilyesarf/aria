#ifndef MENUSAUV_H
#define MENUSAUV_H

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>

#include "../header.h"

/*
extern SDL_Surface *screen, *background;
extern TTF_Font *font;
extern SDL_Color textColor;
extern Mix_Chunk *hoverSound;
extern Button buttons[4];
*/


void initMenuSave(Menu *menus);
void initMenuSaveButtons(Button *buttons);
void initMenuChooseSave(Menu *menus);
void initMenuChooseSaveButtons(Button *buttons);
void renderMenuSave(SDL_Surface *background, SDL_Surface *screen, TTF_Font *font, SDL_Color textColor, Button *buttons, int n_btns);
void renderMenuChooseSave(SDL_Surface *background, SDL_Surface *screen, TTF_Font *font, SDL_Color textColor, Button *buttons, int n_btns);
void handleEventSaveMenu(int *menuState, SDL_Event event, Button *buttons, int n_btns, Mix_Chunk *hoverSound);
void handleEventChooseSaveMenu(int *menuState, SDL_Event event, Button *buttons, int n_btns, Mix_Chunk *hoverSound);
void cleanup(Mix_Chunk *hoverSound, SDL_Surface *background, TTF_Font *font); 

#endif

