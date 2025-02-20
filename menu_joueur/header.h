#ifndef HEADER_H
#define HEADER_H

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define N_BTNS 4

typedef struct {
    SDL_Rect rect;
    char *text;
    int selected;
} Button;

/*
extern SDL_Surface *screen, *background;
extern TTF_Font *font;
extern SDL_Color textColor;
extern Mix_Chunk *hoverSound;
extern Button buttons[4];
*/

SDL_Surface* init_background();
Mix_Chunk* init_audio();
TTF_Font* init_font();
void init_buttons(Button* button);
void renderText(SDL_Surface *screen, const char *text, TTF_Font *font, SDL_Color textColor, int x, int y);
void renderMenuSauv(SDL_Surface *background, SDL_Surface *screen, TTF_Font *font, SDL_Color textColor, Button *buttons);
void renderMenuChooseSave(SDL_Surface *background, SDL_Surface *screen, TTF_Font *font, SDL_Color textColor, Button *buttons);
void handleEvents(int *running, int *menuState, Button *buttons, Mix_Chunk *hoverSound);
void cleanup(Mix_Chunk *hoverSound, SDL_Surface *background, TTF_Font *font); 

#endif

