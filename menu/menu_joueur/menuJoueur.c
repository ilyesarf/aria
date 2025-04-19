
#include "menuJoueur.h"
#include "../header.h"

static int showSubButtons = 0;

void initMenuJoueur(Menu *menus) {
    printf("Init Menu Joueur\n");
    menus[MENU_PLAYER].n_btns = 9;
    menus[MENU_PLAYER].buttons = malloc(menus[MENU_PLAYER].n_btns * sizeof(Button)); // Allocate memory for buttons
    menus[MENU_PLAYER].init_buttons = initMenuJoueurButtons;
    menus[MENU_PLAYER].render = renderMenuJoueur;
    menus[MENU_PLAYER].handleEvent = handleEventJoueurMenu;
    menus[MENU_PLAYER].init_buttons(menus[MENU_PLAYER].buttons);
}

void initMenuJoueurButtons(Button *buttons) {
    printf("Init Menu Joueur Buttons\n");
    buttons[0].normalImage = load_image("./assets/buttons/singleplayer.png");
    buttons[0].hoverImage = load_image("./assets/buttons/singleplayerhov.png");
    buttons[0].rect = (SDL_Rect){SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 50,300,40}; // Single Player
    buttons[0].selected = 0;

    buttons[1].normalImage = load_image("./assets/buttons/multiplayer.png");
    buttons[1].hoverImage = load_image("./assets/buttons/multiplayerhov.png");
    buttons[1].rect = (SDL_Rect){SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 50,300,40}; // Multiplayer
    buttons[1].selected = 0;

    buttons[2].normalImage = load_image("./assets/buttons/return.png");
    buttons[2].hoverImage = load_image("./assets/buttons/returnhov.png");
    buttons[2].rect = (SDL_Rect){SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 150,300,40}; // Return
    buttons[2].selected = 0;

    buttons[3].normalImage = load_image("./assets/buttons/avatar1.png");
    buttons[3].hoverImage = load_image("./assets/buttons/avatar1hov.png");
    buttons[3].rect = (SDL_Rect){SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 150,300,40}; // Avatar 1
    buttons[3].selected = 0;

    buttons[4].normalImage = load_image("./assets/buttons/avatar2.png");
    buttons[4].hoverImage = load_image("./assets/buttons/avatar2hov.png");
    buttons[4].rect = (SDL_Rect){SCREEN_WIDTH / 2 + 100, SCREEN_HEIGHT / 2 - 150,300,40}; // Avatar 2
    buttons[4].selected = 0;

    buttons[5].normalImage = load_image("./assets/buttons/input1.png");
    buttons[5].hoverImage = load_image("./assets/buttons/input1hov.png");
    buttons[5].rect = (SDL_Rect){SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 250,300,40}; // Input 1
    buttons[5].selected = 0;

    buttons[6].normalImage = load_image("./assets/buttons/input2.png");
    buttons[6].hoverImage = load_image("./assets/buttons/input2hov.png");
    buttons[6].rect = (SDL_Rect){SCREEN_WIDTH / 2 + 100, SCREEN_HEIGHT / 2 - 250,300,40}; // Input 2
    buttons[6].selected = 0;

    buttons[7].normalImage = load_image("./assets/buttons/submit.png");
    buttons[7].hoverImage = load_image("./assets/buttons/submithov.png");
    buttons[7].rect = (SDL_Rect){SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 250,300,40}; // Submit
    buttons[7].selected = 0;

    buttons[8].normalImage = load_image("./assets/buttons/return.png");
    buttons[8].hoverImage = load_image("./assets/buttons/returnhov.png");
    buttons[8].rect = (SDL_Rect){SCREEN_WIDTH / 2 + 100, SCREEN_HEIGHT / 2 + 250,300,40}; // Return
    buttons[8].selected = 0;
}

void renderMenuJoueur(SDL_Surface *background, SDL_Surface *screen, TTF_Font *font, SDL_Color textColor, Button *buttons, int n_btns) {
    SDL_BlitSurface(background, NULL, screen, NULL);

    if (!showSubButtons) {
        for (int i = 0; i < 3; i++) {
            renderButton(screen, font, textColor, buttons[i]);
        }
    } else {
        for (int i = 3; i < n_btns; i++) {
            renderButton(screen, font, textColor, buttons[i]);
        }
    }

    SDL_Flip(screen);
}

void handleEventJoueurMenu(int *menuState, SDL_Event event, Button *buttons, int n_btns, Mix_Chunk *hoverSound) {
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
                        if (i == 0 || i == 1) {
                            showSubButtons = 1; // Show sub buttons
                            buttons[0].selected = 0; // Hide main buttons
                            buttons[1].selected = 0;
                            buttons[2].selected = 0;
                        } else if (i == 8) {
                            showSubButtons = 0; // Hide sub buttons
                        } else if(i==7){
                            *menuState = MENU_BEST_SCORE; // Return to best score
                        } else if (i ==2){
                            printf("Return to main menu\n");
                            *menuState = MENU_PRINCIPAL; // Start game
                        }
                    }
                }
            }
        }
    }
}