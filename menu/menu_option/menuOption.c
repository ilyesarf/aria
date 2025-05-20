
#include "menuOption.h"
#include "../header.h"



void initMenuOption(Menu *menus) {
    printf("Init Menu Option\n");
    menus[MENU_OPTION].n_btns = 5;
    menus[MENU_OPTION].buttons = malloc(menus[MENU_OPTION].n_btns * sizeof(Button)); // Allocate memory for buttons
    if (!menus[MENU_OPTION].buttons) {
        fprintf(stderr, "Failed to allocate memory for buttons\n");
        exit(EXIT_FAILURE);
    }

    menus[MENU_OPTION].init_buttons = initMenuOptionButtons;
    menus[MENU_OPTION].render = renderMenuOption;
    menus[MENU_OPTION].handleEvent = handleEventOptionMenu;
    menus[MENU_OPTION].init_buttons(menus[MENU_OPTION].buttons);
}

void initMenuOptionButtons(Button *buttons) {
    buttons[0].text = "Fullscreen";
    buttons[0].rect = (SDL_Rect){600,200,300,40}; // Fullscreen
    buttons[0].selected = 0;

    buttons[1].text = "Windowed";
    buttons[1].rect = (SDL_Rect){1000,200,300,40}; // Windowed
    buttons[1].selected = 0;

    buttons[2].text = "Volume Up";
    buttons[2].rect = (SDL_Rect){600,500,300,40}; // Volume Up
    buttons[2].selected = 0;

    buttons[3].text = "Volume Down";
    buttons[3].rect = (SDL_Rect){1000,500,300,40}; // Volume Down
    buttons[3].selected = 0;

    buttons[4].text = "Return";
    buttons[4].rect = (SDL_Rect){800,800,300,40}; // Return
    buttons[4].selected = 0;

}

void renderVolumeText(SDL_Surface *screen, SDL_Color textColor, int volume) {
    TTF_Font* font = TTF_OpenFont("./assets/fonts/font.ttf", 40);
    char volumeText[20];
    sprintf(volumeText, "Volume: %d", volume);
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, volumeText, textColor);
    if (textSurface) {
        SDL_Rect textLocation = {200, 500, 0, 0}; // Position the text at the top-left corner
        SDL_BlitSurface(textSurface, NULL, screen, &textLocation);
        SDL_FreeSurface(textSurface);
    }
}

void renderMenuOption(SDL_Surface *background, SDL_Surface *butImage, SDL_Surface *screen, TTF_Font *font, SDL_Color textColor, Button *buttons, int n_btns) {
    SDL_BlitSurface(background, NULL, screen, NULL);

    for (int i = 0; i < n_btns; i++) {
        renderButton(screen, butImage, font, textColor, buttons[i]); 
    }
    
    int volume = Mix_VolumeMusic(-1); // Get the current volume
    renderVolumeText(screen,textColor, volume); // Render the volume text


    SDL_Flip(screen);
}
void cleanupMenuOption(Menu *menu) {
    free(menu->buttons);
    menu->buttons = NULL;
}

void handleEventOptionMenu(int *menuState, Save *save, SDL_Event event, Button *buttons, int n_btns, Mix_Chunk *hoverSound) {
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
                            Mix_VolumeMusic(volume + 5);
                        } else if (i == 3) {
                            // Handle Volume Down
                            printf("Volume Down selected\n");
                            int volume = Mix_VolumeMusic(-1);
                            Mix_VolumeMusic(volume - 5);
                        } else if (i == 4) {
                            *menuState = MENU_ENIGME; // Return to main menu
                            printf("Return selected\n");
                        }
                    }
                }
            }
        }
    }
}