#ifndef HEADER_H
#define HEADER_H

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define N_BTNS 4
#define N_MENUS 8

// Menu States
#define QUIT_GAME -1
#define MAIN_GAME 0
#define MENU_PRINCIPAL 1
#define MENU_OPTION 2
#define MENU_SAVE 3
#define MENU_NEW_LOAD_SAVE 4
#define MENU_PLAYER 5
#define MENU_CHOOSE_PLAYER 6
#define MENU_BEST_SCORE 7

typedef struct {
    SDL_Surface *normalImage;
    SDL_Surface *hoverImage;
    SDL_Rect rect;
    const char *text;
    int selected;
} Button;

typedef struct Menu {
    int n_btns;
    Button *buttons;
    void (*init_buttons)(Button *buttons);
    void (*render)(SDL_Surface *background, SDL_Surface *screen, TTF_Font *font, SDL_Color textColor, Button *buttons, int n_btns);
    void (*handleEvent)(int *menuState, SDL_Event event, Button *buttons, int n_btns, Mix_Chunk *hoverSound);
} Menu;

SDL_Surface* init_screen();
void init_audio();
SDL_Surface* load_image(char *filename);
Mix_Chunk* load_sound(char *filename);
TTF_Font* load_font(char *filename);
void renderText(SDL_Surface *screen, const char *text, TTF_Font *font, SDL_Color textColor, int x, int y);
void renderButton(SDL_Surface *screen, TTF_Font *font, SDL_Color textColor, Button button);

void init_menus(Menu *menus);

#endif
