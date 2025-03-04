#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include <stdio.h>
#include "menuOption.h"
#include "../header.h"



void initMenuOption(Menu *menus) {
    printf("Init Menu Option\n");
    menus[MENU_OPTION].n_btns = 5;
    menus[MENU_OPTION].buttons = malloc(menus[MENU_OPTION].n_btns * sizeof(Button)); // Allocate memory for buttons
    menus[MENU_OPTION].init_buttons = initMenuOptionButtons;
    menus[MENU_OPTION].render = renderMenuOption;
    menus[MENU_OPTION].handleEvent = handleEventOptionMenu;
    menus[MENU_OPTION].init_buttons(menus[MENU_OPTION].buttons);
}

void initMenuOptionButtons(Button *buttons) {
    buttons[0].normalImage = load_image("buttons/fullscreen.png");
    buttons[0].hoverImage = load_image("buttons/fullscreenhov.png");
    buttons[0].rect = (SDL_Rect){200,200,300,40}; // Fullscreen
    buttons[0].selected = 0;

    buttons[1].normalImage = load_image("buttons/windowed.png");
    buttons[1].hoverImage = load_image("buttons/windowedhov.png");
    buttons[1].rect = (SDL_Rect){200,350,300,40}; // Windowed
    buttons[1].selected = 0;

    buttons[2].normalImage = load_image("buttons/volumeup.png");
    buttons[2].hoverImage = load_image("buttons/volumeuphov.png");
    buttons[2].rect = (SDL_Rect){200,500,300,40}; // Volume Up
    buttons[2].selected = 0;

    buttons[3].normalImage = load_image("buttons/volumedwn.png");
    buttons[3].hoverImage = load_image("buttons/volumedwnhov.png");
    buttons[3].rect = (SDL_Rect){200,650,300,40}; // Volume Down
    buttons[3].selected = 0;

    buttons[4].normalImage = load_image("buttons/return.png");
    buttons[4].hoverImage = load_image("buttons/returnhov.png");
    buttons[4].rect = (SDL_Rect){200,800,300,40}; // Return
    buttons[4].selected = 0;
}

void renderMenuOption(SDL_Surface *background, SDL_Surface *screen, TTF_Font *font, SDL_Color textColor, Button *buttons, int n_btns) {
    SDL_BlitSurface(background, NULL, screen, NULL);

    for (int i = 0; i < n_btns; i++) {
        renderButton(screen, font, textColor, buttons[i]);
    }

    SDL_Flip(screen);
}

void handleEventOptionMenu(int *menuState, SDL_Event event, Button *buttons, int n_btns, Mix_Chunk *hoverSound) {
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
                            // Handle Fullscreen
                            printf("Fullscreen selected\n");
                            if (!(SDL_GetVideoSurface()->flags & SDL_FULLSCREEN)) {
                                SDL_WM_ToggleFullScreen(SDL_GetVideoSurface());
                            }
                        } else if (i == 1) {
                            // Handle Windowed
                            printf("Windowed selected\n");
                            if (SDL_GetVideoSurface()->flags & SDL_FULLSCREEN) {
                                SDL_WM_ToggleFullScreen(SDL_GetVideoSurface());
                            }
                        } else if (i == 2) {
                            // Handle Volume Up
                            printf("Volume Up selected\n");
                            int volume = Mix_VolumeMusic(-1);
                            Mix_VolumeMusic(volume + 10);
                        } else if (i == 3) {
                            // Handle Volume Down
                            printf("Volume Down selected\n");
                            int volume = Mix_VolumeMusic(-1);
                            Mix_VolumeMusic(volume - 10);
                        } else if (i == 4) {
                            *menuState = MENU_PRINCIPAL; // Return to main menu
                        }
                    }
                }
            }
        }
    }
}