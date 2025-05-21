#include "menuJoueur.h"
#include "../header.h"

static int showSubButtons = 0;

void initMenuJoueur(Menu *menus) {
    printf("Init Menu Joueur\n");
    menus[MENU_PLAYER].n_btns = 9;
    menus[MENU_PLAYER].buttons = malloc(menus[MENU_PLAYER].n_btns * sizeof(Button)); // Allocate memory for buttons
    if (!menus[MENU_PLAYER].buttons) {
        fprintf(stderr, "Failed to allocate memory for buttons\n");
        exit(EXIT_FAILURE);
    }

    menus[MENU_PLAYER].init_buttons = initMenuJoueurButtons;
    menus[MENU_PLAYER].render = renderMenuJoueur;
    menus[MENU_PLAYER].handleEvent = handleEventJoueurMenu;
    menus[MENU_PLAYER].init_buttons(menus[MENU_PLAYER].buttons);
}

void initMenuJoueurButtons(Button *buttons) {
    printf("Init Menu Joueur Buttons\n");

    buttons[0].text = "Single Player";
    buttons[0].rect = (SDL_Rect){SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 50,300,40}; // Single Player
    buttons[0].selected = 0;

    buttons[1].text = "Multiplayer";
    buttons[1].rect = (SDL_Rect){SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 50,300,40}; // Multiplayer
    buttons[1].selected = 0;

    buttons[2].text = "Return";
    buttons[2].rect = (SDL_Rect){SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 150,300,40}; // Return
    buttons[2].selected = 0;

    buttons[3].text = "Avatar 1";
    buttons[3].rect = (SDL_Rect){SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 150,300,40}; // Avatar 1
    buttons[3].selected = 0;

    buttons[4].text = "Avatar 2";
    buttons[4].rect = (SDL_Rect){SCREEN_WIDTH / 2 + 100, SCREEN_HEIGHT / 2 - 150,300,40}; // Avatar 2
    buttons[4].selected = 0;

    buttons[5].text = "Keyboard";
    buttons[5].rect = (SDL_Rect){SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 250,300,40}; // Input 1
    buttons[5].selected = 0;

    buttons[6].text = "Controller";
    buttons[6].rect = (SDL_Rect){SCREEN_WIDTH / 2 + 100, SCREEN_HEIGHT / 2 - 250,300,40}; // Input 2
    buttons[6].selected = 0;

    buttons[7].text = "Submit";
    buttons[7].rect = (SDL_Rect){SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 250,300,40}; // Submit
    buttons[7].selected = 0;

    buttons[8].text = "Return";
    buttons[8].rect = (SDL_Rect){SCREEN_WIDTH / 2 + 100, SCREEN_HEIGHT / 2 + 250,300,40}; // Return
    buttons[8].selected = 0;
}

void renderMenuJoueur(SDL_Surface *background, SDL_Surface *butImage, SDL_Surface *screen, TTF_Font *font, SDL_Color textColor, Button *buttons, int n_btns) {
    SDL_BlitSurface(background, NULL, screen, NULL);

    if (!showSubButtons) {
        for (int i = 0; i < 3; i++) {
            renderButton(screen, butImage, font, textColor, buttons[i]);
        }
    } else {
        for (int i = 3; i < n_btns; i++) {
            renderButton(screen, butImage, font, textColor, buttons[i]);
        }
    }

    SDL_Flip(screen);
}

void handleEventJoueurMenu(int *menuState, Save *save, SDL_Event event, Button *buttons, int n_btns, Mix_Chunk *hoverSound) {
    static int selectedPlayerCount = 1;  // Default to single player
    static int selectedInputMethod = 0;  // Default to keyboard
    static int selectedAvatar = 0;       // Default to avatar 1
    
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
                        if (i == 0) { // Single Player
                            selectedPlayerCount = 1;
                            showSubButtons = 1; // Show sub buttons
                            buttons[0].selected = 0; // Hide main buttons
                            buttons[1].selected = 0;
                            buttons[2].selected = 0;
                        } else if (i == 1) { // Multiplayer
                            selectedPlayerCount = 2;
                            showSubButtons = 1; // Show sub buttons
                            buttons[0].selected = 0; // Hide main buttons
                            buttons[1].selected = 0;
                            buttons[2].selected = 0;
                        } else if (i == 8) { // Return from sub-menu
                            showSubButtons = 0; // Hide sub buttons
                        } else if (i == 7) { // Submit
                            // Save the player configuration
                            save->num_players = selectedPlayerCount;
                            save->input_methods[0] = selectedInputMethod;
                            
                            // If two players, initialize the second player
                            if (selectedPlayerCount == 2) {
                                // Allocate memory for two players if needed
                                if (save->players == NULL || save->num_players != 2) {
                                    save->players = realloc(save->players, 2 * sizeof(Player));
                                    
                                    // Initialize the second player by copying the first one
                                    save->players[1] = save->players[0];
                                    save->players[1].pos.x = SCREEN_WIDTH / 4 + 100;
                                    
                                    // Load the second player's image
                                    if (save->players[1].img) {
                                        SDL_FreeSurface(save->players[1].img);
                                    }
                                    save->players[1].img = IMG_Load("./game/assets/player/2.png");
                                }
                                
                                // Set default input method for second player (controller)
                                save->input_methods[1] = 1;
                            }
                            
                            printf("Player configuration saved: %d players, input method: %d\n", 
                                   save->num_players, save->input_methods[0]);
                            
                            *menuState = MAIN_GAME; // Start the game
                        } else if (i == 2) { // Return to main menu
                            printf("Return to main menu\n");
                            *menuState = MENU_PRINCIPAL;
                        } else if (i == 3 || i == 4) { // Avatar selection
                            selectedAvatar = i - 3;
                            printf("Selected avatar: %d\n", selectedAvatar + 1);
                        } else if (i == 5 || i == 6) { // Input method selection
                            selectedInputMethod = i - 5;
                            printf("Selected input method: %s\n", 
                                   selectedInputMethod == 0 ? "Keyboard" : "Controller");
                        }
                    }
                }
            }
        }
    }
}

void cleanupMenuJoueur(Menu *menu) {
    free(menu->buttons);
    menu->buttons = NULL;
}