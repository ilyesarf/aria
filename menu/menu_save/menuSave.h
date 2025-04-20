#ifndef MENUSAUV_H
#define MENUSAUV_H

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
void cleanupMenuSave(Menu *menu) ;

#endif

