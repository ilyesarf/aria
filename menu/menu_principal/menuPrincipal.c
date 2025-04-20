
#include "menuPrincipal.h"
#include "../header.h"

void initMenuPrincipal(Menu *menus) {
    printf("Init Menu Principal\n");
    menus[MENU_PRINCIPAL].n_btns = 4;
    menus[MENU_PRINCIPAL].buttons = malloc(menus[MENU_PRINCIPAL].n_btns * sizeof(Button)); // Allocate memory for buttons
    menus[MENU_PRINCIPAL].init_buttons = initMenuPrincipalButtons;
    menus[MENU_PRINCIPAL].render = renderMenuPrincipal;
    menus[MENU_PRINCIPAL].handleEvent = handleEventPrincipalMenu;
    menus[MENU_PRINCIPAL].init_buttons(menus[MENU_PRINCIPAL].buttons);
}

void initMenuPrincipalButtons(Button *buttons) {
    buttons[0].normalImage = load_image("./assets/buttons/avatar1.png");
    buttons[0].hoverImage = load_image("./assets/buttons/avatar1hov.png");
    buttons[0].rect = (SDL_Rect){700,200,300,40}; // start
    buttons[0].selected = 0;

    buttons[1].normalImage = load_image("./assets/buttons/avatar1.png");
    buttons[1].hoverImage = load_image("./assets/buttons/avatar1hov.png");
    buttons[1].rect = (SDL_Rect){700,350,300,40}; // Button 2
    buttons[1].selected = 0;

    buttons[2].normalImage = load_image("./assets/buttons/avatar1.png");
    buttons[2].hoverImage = load_image("./assets/buttons/avatar1hov.png");
    buttons[2].rect = (SDL_Rect){700,500,300,40}; // Button 3
    buttons[2].selected = 0;

    buttons[3].normalImage = load_image("./assets/buttons/quit.png");
    buttons[3].hoverImage = load_image("./assets/buttons/quithov.png");
    buttons[3].rect = (SDL_Rect){700,650,300,40}; // Return
    buttons[3].selected = 0;
}

void renderMenuPrincipal(SDL_Surface *background, SDL_Surface *screen, TTF_Font *font, SDL_Color textColor, Button *buttons, int n_btns) {
    SDL_BlitSurface(background, NULL, screen, NULL);

    for (int i = 0; i < n_btns; i++) {
        renderButton(screen, font, textColor, buttons[i]);     
        printf("Button %d: rect=(%d, %d, %d, %d), selected=%d\n",
               i, buttons[i].rect.x, buttons[i].rect.y, buttons[i].rect.w, buttons[i].rect.h, buttons[i].selected);
    }

    printf("All buttons rendered !!\n");

    SDL_Flip(screen);
}

void handleEventPrincipalMenu(int *menuState, SDL_Event event, Button *buttons, int n_btns, Mix_Chunk *hoverSound) {
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
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (event.button.button == SDL_BUTTON_LEFT) {
                for (int i = 0; i < n_btns; i++) {
                    if (buttons[i].selected) {
                        if (i == 0) {
                            *menuState = MENU_SAVE; // First button
                        } else if (i == 1) {
                            *menuState = MENU_OPTION; // Options button
                        } else if (i == 2) {
                            *menuState = MENU_BEST_SCORE; // Joueur button
                        } else if (i == 3) {
                            *menuState = QUIT_GAME; // Quit button
                        }
                    }
                }
            }
        }
    }
}