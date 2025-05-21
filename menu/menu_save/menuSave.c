#include "menuSave.h"
#include "../header.h"

void initMenuSave(struct Menu *menus){
    printf("Init Menu Save\n");
    menus[MENU_SAVE].n_btns = 2;
    menus[MENU_SAVE].buttons = malloc(menus[MENU_SAVE].n_btns * sizeof(Button)); // Allocate memory for buttons
    if (!menus[MENU_SAVE].buttons) {
        fprintf(stderr, "Failed to allocate memory for buttons\n");
        exit(EXIT_FAILURE);
    }
    menus[MENU_SAVE].init_buttons = initMenuSaveButtons;
    menus[MENU_SAVE].render = renderMenuSave;
    menus[MENU_SAVE].handleEvent = handleEventSaveMenu;
    menus[MENU_SAVE].init_buttons(menus[MENU_SAVE].buttons);
    //printf("button 1 text: %s\n", menus[MENU_SAVE].buttons[0].text);
}

void initMenuSaveButtons(Button *buttons){
    buttons[0].rect = (SDL_Rect){800, 200,300,40}; // OUI
    buttons[0].text = "Save";
    buttons[0].selected = 0;


    buttons[1].rect = (SDL_Rect){800, 350,300,40}; // NON
    buttons[1].text = "Resume";
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

    buttons[0].text = "Load Game";
    buttons[0].rect = (SDL_Rect){800, 200,300,40} ;
    buttons[0].selected = 0;

    buttons[1].text = "New Game";
    buttons[1].rect = (SDL_Rect){800, 350,300,40};
    buttons[1].selected = 0;
}

void renderMenuSave(SDL_Surface *background, SDL_Surface *butImage, SDL_Surface *screen, TTF_Font *font, SDL_Color textColor, Button *buttons, int n_btns) {
    SDL_BlitSurface(background, NULL, screen, NULL);
    for (int i = 0; i < n_btns; i++) {
        renderButton(screen, butImage, font, textColor, buttons[i]); 
    }

    SDL_Flip(screen);
}

void renderMenuChooseSave(SDL_Surface *background, SDL_Surface *butImage, SDL_Surface *screen, TTF_Font *font, SDL_Color textColor, Button *buttons, int n_btns) {
    SDL_BlitSurface(background, NULL, screen, NULL); // Draw background

    for (int i = 0; i < n_btns; i++) {
        renderButton(screen, butImage, font, textColor, buttons[i]);
    }

    SDL_Flip(screen);
}

void handleEventSaveMenu(int *menuState, Save *save, SDL_Event event, Button *buttons, int n_btns, Mix_Chunk *hoverSound) {
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
                for (int i = 0; i < 2; i++) {
                    if (buttons[i].selected) {
                        if (i == 0) {
                            Mix_PlayChannel(-1, hoverSound, 0);
                            save_game("savegame.dat", save);
                            *menuState = MENU_PRINCIPAL; // OUI -> Next menu
                        } else if (i == 1) { 
                            Mix_PlayChannel(-1, hoverSound, 0);
                            *menuState = MAIN_GAME; // NON -> Exit
                            printf("Resume game. menuState = %d\n", *menuState);
                        }
                    }
                }
            }
        }
    }
}
void cleanupMenuSave(Menu *menu) {
    free(menu->buttons);
    menu->buttons = NULL;
}

void handleEventChooseSaveMenu(int *menuState, Save* save, SDL_Event event, Button *buttons, int n_btns, Mix_Chunk *hoverSound) {
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
                    if (i == 0) {
                        load_game("savegame.dat", save);
                        *menuState = MAIN_GAME; // Charger jeu
                    }
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

void load_game(char *filename, Save *save) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        //fprintf(stderr, "Failed to open file: %s\n", filename);
        return;
    }
    
    // Read number of players and input methods
    int num_players;
    int input_methods[2];
    
    fread(&num_players, sizeof(int), 1, file);
    fread(input_methods, sizeof(int), 2, file);
    
    // Update save structure with player configuration
    save->num_players = num_players;
    save->input_methods[0] = input_methods[0];
    save->input_methods[1] = input_methods[1];
    
    // Ensure we have enough memory allocated for all players
    if (num_players > 1 && save->num_players != 2) {
        // Allocate memory for second player if needed
        save->players = realloc(save->players, 2 * sizeof(Player));
        
        // Initialize the second player with default values
        // Copy values from first player but change position
        save->players[1] = save->players[0];
        save->players[1].pos.x = SCREEN_WIDTH / 4 + 100;
        
        // Load the second player's image
        if (save->players[1].img) {
            SDL_FreeSurface(save->players[1].img);
        }
        save->players[1].img = IMG_Load("./game/assets/player/2.png");
    }
    
    // Read player data for each player
    for (int i = 0; i < num_players; i++) {
        Player player_data;
        
        // Read player data
        if (fread(&player_data, sizeof(Player), 1, file) == 1) {
            // Copy player position, health, lives, and score
            save->players[i].pos.x = player_data.pos.x;
            save->players[i].pos.y = player_data.pos.y;
            save->players[i].health = player_data.health;
            save->players[i].lives = player_data.lives;
            save->players[i].score = player_data.score;
            
            // Don't copy the image pointer, keep using the existing one
            
            printf("Player %d loaded from %s\n", i+1, filename);
            printf("Player %d position: %d, %d\n", i+1, save->players[i].pos.x, save->players[i].pos.y);
            printf("Player %d health: %d, lives: %d, score: %d\n", i+1,
                   save->players[i].health, save->players[i].lives, save->players[i].score);
        }
    }
    
    fclose(file);
}

void save_game(char *filename, Save *save) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        fprintf(stderr, "Failed to open file: %s\n", filename);
        return;
    }
    
    // Write player configuration
    fwrite(&save->num_players, sizeof(int), 1, file);
    fwrite(save->input_methods, sizeof(int), 2, file);
    
    // Write data for each player
    for (int i = 0; i < save->num_players; i++) {
        // Write player data
        fwrite(&save->players[i], sizeof(Player), 1, file);
        printf("Player %d saved to %s\n", i+1, filename);
    }
    
    fclose(file);
    printf("Game saved to %s\n", filename); 
}