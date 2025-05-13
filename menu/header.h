#ifndef HEADER_H
#define HEADER_H


#include "../helpers.h"

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
#define MENU_ENIGME 8
#define MENU_QUIZ 9
#define MENU_PUZZLE 10

typedef struct {
    //SDL_Surface *normalImage;
    //SDL_Surface *hoverImage;
    SDL_Rect rect;
    const char *text;
    int selected;
} Button;

typedef struct Menu {
    int n_btns;
    Button *buttons;
    void (*init_buttons)(Button *buttons);
    void (*render)(SDL_Surface *background, SDL_Surface *butImage, SDL_Surface *screen, TTF_Font *font, SDL_Color textColor, Button *buttons, int n_btns);
    void (*handleEvent)(int *menuState, Save save, SDL_Event event, Button *buttons, int n_btns, Mix_Chunk *hoverSound);
} Menu;

void renderText(SDL_Surface *screen, const char *text, TTF_Font *font, SDL_Color textColor, int x, int y);
void renderButton(SDL_Surface *screen, SDL_Surface *butImage, TTF_Font *font, SDL_Color textColor, Button button);

void init_menus(Menu *menus);

int menu(SDL_Surface *screen, SDL_Surface *background, TTF_Font *font, SDL_Color textColor, SDL_Surface *butImage, Mix_Chunk *hoverSound, Mix_Music *musique, int menuState, Save save, Menu *menus);

void cleanup(Mix_Chunk *hoverSound, Mix_Music *musique, SDL_Surface *background, TTF_Font *font, Menu *menus);

#endif
