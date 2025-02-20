#include "header.h"

int main() {
    SDL_Surface *screen; 
    screen = init_screen();
    SDL_Surface *background;
    background = init_background();
    TTF_Font *font;
    font = init_font();
    SDL_Color textColor = {0,0,0}; // black text
    Mix_Chunk *hoverSound;
    hoverSound = init_audio();

    Button buttons[N_BTNS];
    init_buttons(buttons);

    int running = 1, menuState = 1;

    while (running) {
        handleEvents(&running, &menuState, buttons, hoverSound);
        printf("menu state: %d\n", menuState);
        if (menuState==1){
            printf("Menu Save!\n");
            renderMenuSauv(background, screen, font, textColor, buttons);
        } else if (menuState==2){
            printf("Menu Choose Save!\n");
            renderMenuChooseSave(background, screen, font, textColor, buttons);
        } else if (menuState==3){
            printf("New game...\n");
        } else{
            printf("Load game...\n");
        }
        SDL_Delay(16);
    }

    cleanup(hoverSound, background, font);
    return 0;
}
