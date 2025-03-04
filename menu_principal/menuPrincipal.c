#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include <stdio.h>
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
    buttons[0].normalImage = load_image("buttons/avatar1.png");
    buttons[0].hoverImage = load_image("buttons/avatar1hov.png");
    buttons[0].rect = (SDL_Rect){SCREEN_WIDTH / 2 - 75, SCREEN_HEIGHT / 2 - 100, 150, 40}; // Button 1
    buttons[0].selected = 0;

    buttons[1].normalImage = load_image("buttons/avatar1.png");
    buttons[1].hoverImage = load_image("buttons/avatar1hov.png");
    buttons[1].rect = (SDL_Rect){SCREEN_WIDTH / 2 - 75, SCREEN_HEIGHT / 2 - 50, 150, 40}; // Button 2
    buttons[1].selected = 0;

    buttons[2].normalImage = load_image("buttons/avatar1.png");
    buttons[2].hoverImage = load_image("buttons/avatar1hov.png");
    buttons[2].rect = (SDL_Rect){SCREEN_WIDTH / 2 - 75, SCREEN_HEIGHT / 2, 150, 40}; // Button 3
    buttons[2].selected = 0;

    buttons[3].normalImage = load_image("buttons/return.png");
    buttons[3].hoverImage = load_image("buttons/returnhov.png");
    buttons[3].rect = (SDL_Rect){SCREEN_WIDTH / 2 - 75, SCREEN_HEIGHT / 2 + 50, 150, 40}; // Return
    buttons[3].selected = 0;
}

void renderMenuPrincipal(SDL_Surface *background, SDL_Surface *screen, TTF_Font *font, SDL_Color textColor, Button *buttons, int n_btns) {
    SDL_BlitSurface(background, NULL, screen, NULL);

    for (int i = 0; i < n_btns; i++) {
        renderButton(screen, font, textColor, buttons[i]);
    }

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