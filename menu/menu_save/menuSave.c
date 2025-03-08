#include "menuSave.h"
#include "../header.h"

void initMenuSave(struct Menu *menus){
    printf("Init Menu Save\n");
    menus[MENU_SAVE].n_btns = 2;

    menus[MENU_SAVE].buttons = malloc(menus[MENU_SAVE].n_btns * sizeof(Button)); // Allocate memory for buttons
    menus[MENU_SAVE].init_buttons = initMenuSaveButtons;
    menus[MENU_SAVE].render = renderMenuSave;
    menus[MENU_SAVE].handleEvent = handleEventSaveMenu;
    menus[MENU_SAVE].init_buttons(menus[MENU_SAVE].buttons);
    //printf("button 1 text: %s\n", menus[MENU_SAVE].buttons[0].text);
}

void initMenuSaveButtons(Button *buttons){
    buttons[0].normalImage = load_image("buttons/yes.png");
    buttons[0].hoverImage = load_image("buttons/yeshov.png");
    buttons[0].rect = (SDL_Rect){800, 200,300,40}; // OUI
    //buttons[0].text = "OUI";
    buttons[0].selected = 0;


    buttons[1].normalImage = load_image("buttons/no.png");
    buttons[1].hoverImage = load_image("buttons/nohov.png");
    buttons[1].rect = (SDL_Rect){800, 350,300,40}; // NON
    //buttons[1].text = "NON";
    buttons[1].selected = 0;
}

void initMenuChooseSave(Menu *menus){
    menus[MENU_NEW_LOAD_SAVE].n_btns = 2;
    menus[MENU_NEW_LOAD_SAVE].buttons = malloc(menus[MENU_NEW_LOAD_SAVE].n_btns * sizeof(Button)); // Allocate memory for buttons
    menus[MENU_NEW_LOAD_SAVE].init_buttons = initMenuChooseSaveButtons;
    menus[MENU_NEW_LOAD_SAVE].render = renderMenuChooseSave;
    menus[MENU_NEW_LOAD_SAVE].handleEvent = handleEventChooseSaveMenu;
    menus[MENU_NEW_LOAD_SAVE].init_buttons(menus[MENU_NEW_LOAD_SAVE].buttons);
}

void initMenuChooseSaveButtons(Button *buttons) {

    buttons[0].normalImage = load_image("buttons/load.png");
    buttons[0].hoverImage = load_image("buttons/loadhov.png");
    buttons[0].rect = (SDL_Rect){800, 200,300,40} ;
    buttons[0].selected = 0;

    buttons[1].normalImage = load_image("buttons/newgame.png");
    buttons[1].hoverImage = load_image("buttons/newgamehov.png");
    buttons[1].rect = (SDL_Rect){800, 350,300,40};
    buttons[1].selected = 0;
}

void renderMenuSave(SDL_Surface *background, SDL_Surface *screen, TTF_Font *font, SDL_Color textColor, Button *buttons, int n_btns) {
    SDL_BlitSurface(background, NULL, screen, NULL);
    for (int i = 0; i < n_btns; i++) {
        renderButton(screen, font, textColor, buttons[i]); 
    }

    SDL_Flip(screen);
}

void renderMenuChooseSave(SDL_Surface *background, SDL_Surface *screen, TTF_Font *font, SDL_Color textColor, Button *buttons, int n_btns) {
    SDL_BlitSurface(background, NULL, screen, NULL); // Draw background

    for (int i = 0; i < n_btns; i++) {
        renderButton(screen, font, textColor, buttons[i]);
    }

    SDL_Flip(screen);
}

void handleEventSaveMenu(int *menuState, SDL_Event event, Button *buttons, int n_btns, Mix_Chunk *hoverSound) {
    while (SDL_PollEvent(&event)) {

        if (event.type == SDL_QUIT) *menuState = QUIT_GAME;
        if (event.type == SDL_MOUSEMOTION) {
            for (int i = 0; i < n_btns; i++) {
                if (event.motion.x >= buttons[i].rect.x && event.motion.x <= buttons[i].rect.x + buttons[i].rect.w &&
                    event.motion.y >= buttons[i].rect.y && event.motion.y <= buttons[i].rect.y + buttons[i].rect.h) {
                    buttons[i].selected = 1;
                } else {
                    buttons[i].selected = 0;
                }
            }
        }
    }
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            for (int i = 0; i < 2; i++) {
                if (buttons[i].selected) {
                    if (i == 0) {
                        Mix_PlayChannel(-1, hoverSound, 0);
                        *menuState = MENU_NEW_LOAD_SAVE; // OUI -> Next menu
                    } else if (i == 1) { 
                        Mix_PlayChannel(-1, hoverSound, 0);
                        *menuState = MENU_PRINCIPAL; // NON -> Exit
                    }
                }
            }
        }
    }
}

void handleEventChooseSaveMenu(int *menuState, SDL_Event event, Button *buttons, int n_btns, Mix_Chunk *hoverSound) {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_MOUSEMOTION) {
            for (int i = 0; i < n_btns; i++) {
                if (event.motion.x >= buttons[i].rect.x && event.motion.x <= buttons[i].rect.x + buttons[i].rect.w &&
                    event.motion.y >= buttons[i].rect.y && event.motion.y <= buttons[i].rect.y + buttons[i].rect.h) {
                    buttons[i].selected = 1;
                } else {
                    buttons[i].selected = 0;
                }
            }
        }
    }
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            for (int i = 0; i < 2; i++) {
                if (buttons[i].selected) { 
                    if (i == 0) *menuState = MAIN_GAME; // Charger jeu
                         
                    else if (i == 1) *menuState = MENU_PLAYER; //Nouvelle jeu
                }
            }
        }
    }
    if (event.type == SDL_KEYDOWN) {
        switch(event.key.keysym.sym) {
            case SDLK_n:
                *menuState = MENU_PLAYER;
                break;
            case SDLK_0:
                printf("0\n");
                break;
            default:
                break;
        }
    }
}

void cleanup(Mix_Chunk *hoverSound, SDL_Surface *background, TTF_Font *font) {
    Mix_FreeChunk(hoverSound);
    SDL_FreeSurface(background);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
}
